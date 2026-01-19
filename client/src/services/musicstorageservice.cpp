#include "musicstorageservice.h"
#include "metadataextractor.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QDirIterator>
#include <QRegularExpression>
#include <QBuffer>
#include <QDebug>

MusicStorageService* MusicStorageService::s_instance = nullptr;

MusicStorageService::MusicStorageService(QObject *parent)
    : QObject(parent)
{
    initializeMusicDirectory();
    loadPlaylistData();
}

MusicStorageService::~MusicStorageService()
{
}

MusicStorageService* MusicStorageService::instance()
{
    if (!s_instance) {
        s_instance = new MusicStorageService();
    }
    return s_instance;
}

void MusicStorageService::initializeMusicDirectory()
{
    m_musicDirectory = getStandardMusicPath();
    ensureMusicDirectoryExists();
}

QString MusicStorageService::getStandardMusicPath()
{
    // Get application data directory
    QString appDataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);

    // Create EKNMusic/songs subdirectory
    return appDataPath + "/songs";
}

bool MusicStorageService::ensureMusicDirectoryExists()
{
    QDir dir;
    if (!dir.exists(m_musicDirectory)) {
        return dir.mkpath(m_musicDirectory);
    }
    return true;
}

QList<Track> MusicStorageService::getDownloadedTracks()
{
    QList<Track> tracks;

    if (!ensureMusicDirectoryExists()) {
        return tracks;
    }

    // First, scan the directory for all music files
    QMap<QString, Track> filePathToTrack;
    QDir musicDir(m_musicDirectory);
    QStringList filters;
    filters << "*.mp3" << "*.flac" << "*.wav" << "*.ogg" << "*.m4a";

    QDirIterator it(m_musicDirectory, filters,
                    QDir::Files | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        Track track = extractMetadataFromFile(filePath);
        if (track.isValid()) {
            filePathToTrack[filePath] = track;

            // Update playlist data with current metadata if not exists
            if (!m_playlistData.hasTrackData(filePath)) {
                updateTrackMetadata(filePath, track);
            }
        }
    }

    // Get ordered list from playlist data
    QList<TrackData> orderedData = m_playlistData.getAllTracksOrdered();

    // First add tracks in saved order
    for (const TrackData &data : orderedData) {
        if (filePathToTrack.contains(data.filePath)) {
            Track track = filePathToTrack[data.filePath];
            // Use saved metadata if available
            if (!data.title.isEmpty()) {
                track.setTitle(data.title);
            }
            if (!data.artist.isEmpty()) {
                track.setArtist(data.artist);
            }
            if (!data.album.isEmpty()) {
                track.setAlbum(data.album);
            }
            if (data.dateAdded.isValid()) {
                track.setDateAdded(data.dateAdded);
            }
            if (data.fileSize > 0) {
                track.setFileSize(data.fileSize);
            }
            tracks.append(track);
            filePathToTrack.remove(data.filePath);
        }
    }

    // Add any new tracks not in saved order (at the end)
    for (const Track &track : filePathToTrack.values()) {
        tracks.append(track);
        updateTrackMetadata(track.filePath(), track);
    }

    return tracks;
}

Track MusicStorageService::extractMetadataFromFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists()) {
        qWarning() << "File does not exist:" << filePath;
        return Track();
    }

    qDebug() << "Extracting metadata from:" << filePath;

    // Use MetadataExtractor to get real metadata from the audio file
    MetadataExtractor extractor;
    Track track = extractor.extractMetadata(filePath);

    if (!track.isValid()) {
        qWarning() << "Failed to extract metadata, using filename fallback";

        // Fallback: Parse filename for basic metadata
        QString title = fileInfo.completeBaseName();
        QString artist = "Unknown Artist";
        QString album = "Unknown Album";

        QStringList parts = title.split(QRegularExpression(" [-–] "));
        if (parts.size() >= 2) {
            artist = parts[0].trimmed();
            title = parts[1].trimmed();
        }

        track = Track(filePath, title, artist, album, 0);

        // Look for cover in directory as fallback
        QDir dir = fileInfo.dir();
        QStringList imageFilters;
        imageFilters << "cover.jpg" << "cover.png" << "folder.jpg" << "folder.png";

        QStringList imageFiles = dir.entryList(imageFilters, QDir::Files);
        if (!imageFiles.isEmpty()) {
            track.setAlbumArtPath(dir.absoluteFilePath(imageFiles.first()));
        }
    }

    qDebug() << "Extracted track:" << track.title() << "by" << track.artist();
    return track;
}

bool MusicStorageService::saveTrack(const QString &sourceFilePath,
                                     const Track &trackInfo)
{
    if (!ensureMusicDirectoryExists()) {
        return false;
    }

    QFileInfo sourceInfo(sourceFilePath);
    if (!sourceInfo.exists()) {
        return false;
    }

    // Create destination filename
    QString filename = trackInfo.title().isEmpty()
                       ? sourceInfo.fileName()
                       : trackInfo.title() + "." + sourceInfo.suffix();

    // Sanitize filename (remove invalid characters)
    filename.replace(QRegularExpression("[<>:\"/\\|?*]"), "_");

    QString destPath = m_musicDirectory + "/" + filename;

    // Copy file
    QFile::remove(destPath); // Remove if exists
    bool success = QFile::copy(sourceFilePath, destPath);

    if (success) {
        emit tracksChanged();
    }

    return success;
}

bool MusicStorageService::deleteTrack(const QString &filePath)
{
    // Also delete the album art file if it exists
    QFileInfo fileInfo(filePath);
    QString albumArtPath = fileInfo.absolutePath() + "/." +
                           fileInfo.completeBaseName() + "_cover.jpg";

    // Remove album art file (silently fail if doesn't exist)
    QFile::remove(albumArtPath);

    // Remove the main track file
    bool success = QFile::remove(filePath);

    if (success) {
        // Remove from playlist data
        m_playlistData.removeTrack(filePath);
        savePlaylistData();

        emit tracksChanged();
    }

    return success;
}

QString MusicStorageService::playlistDataFilePath() const
{
    // Get parent directory (one level up from songs folder)
    QDir musicDir(m_musicDirectory);
    musicDir.cdUp(); // Go up to EKNMusic folder

    QString metadataDir = musicDir.absolutePath() + "/metadata";

    // Ensure metadata directory exists
    QDir().mkpath(metadataDir);

    return metadataDir + "/playlist.json";
}

void MusicStorageService::savePlaylistData()
{
    QString filePath = playlistDataFilePath();
    if (!m_playlistData.saveToFile(filePath)) {
        qWarning() << "Failed to save playlist data";
    }
}

void MusicStorageService::loadPlaylistData()
{
    QString filePath = playlistDataFilePath();
    m_playlistData.loadFromFile(filePath);
}

void MusicStorageService::updateTrackOrder(const QList<QString> &orderedFilePaths)
{
    m_playlistData.updateOrder(orderedFilePaths);
    savePlaylistData();
    qDebug() << "Track order updated and saved";
}

void MusicStorageService::updateTrackMetadata(const QString &filePath, const Track &track)
{
    TrackData data;
    data.filePath = filePath;
    data.title = track.title();
    data.artist = track.artist();
    data.album = track.album();
    data.duration = track.duration();
    data.dateAdded = track.dateAdded();
    data.fileSize = track.fileSize();

    // Preserve existing order index if available
    if (m_playlistData.hasTrackData(filePath)) {
        data.orderIndex = m_playlistData.getTrackData(filePath).orderIndex;
    } else {
        // New track, add at the end
        data.orderIndex = m_playlistData.getAllTracksOrdered().size();
    }

    m_playlistData.setTrackData(filePath, data);
    savePlaylistData();
    qDebug() << "Track metadata updated and saved:" << track.title();
}
