#include "musicstorageservice.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QFile>
#include <QDirIterator>
#include <QRegularExpression>
#include <QBuffer>

MusicStorageService* MusicStorageService::s_instance = nullptr;

MusicStorageService::MusicStorageService(QObject *parent)
    : QObject(parent)
{
    initializeMusicDirectory();
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
            tracks.append(track);
        }
    }

    return tracks;
}

Track MusicStorageService::extractMetadataFromFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists()) {
        return Track();
    }

    QString title = fileInfo.completeBaseName();
    QString artist = "Unknown Artist";
    QString album = "Unknown Album";
    qint64 duration = 0;
    QString albumArtPath;

    // Parse filename for basic metadata
    QString baseName = fileInfo.completeBaseName();
    QStringList parts = baseName.split(QRegularExpression(" [-–] "));
    if (parts.size() >= 2) {
        artist = parts[0].trimmed();
        title = parts[1].trimmed();
    }

    // If no embedded art, look for cover in directory
    if (albumArtPath.isEmpty()) {
        QDir dir = fileInfo.dir();
        QStringList imageFilters;
        imageFilters << "cover.jpg" << "cover.png" << "folder.jpg" << "folder.png"
                     << "*.jpg" << "*.png" << "*.jpeg";

        QStringList imageFiles = dir.entryList(imageFilters, QDir::Files);
        if (!imageFiles.isEmpty()) {
            albumArtPath = dir.absoluteFilePath(imageFiles.first());
        }
    }

    Track track(filePath, title, artist, album, duration);
    track.setAlbumArtPath(albumArtPath);
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
    bool success = QFile::remove(filePath);

    if (success) {
        emit tracksChanged();
    }

    return success;
}
