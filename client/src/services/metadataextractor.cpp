#include "metadataextractor.h"
#include <QEventLoop>
#include <QTimer>
#include <QFileInfo>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <QMediaMetaData>

MetadataExtractor::MetadataExtractor(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_metadataReady(false)
{
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.0); // Mute during metadata extraction

    connect(m_player, &QMediaPlayer::metaDataChanged,
            this, &MetadataExtractor::onMetadataChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &MetadataExtractor::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, &MetadataExtractor::onErrorOccurred);
}

MetadataExtractor::~MetadataExtractor()
{
}

Track MetadataExtractor::extractMetadata(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "File does not exist:" << filePath;
        return Track();
    }

    // Reset state
    m_currentFilePath = filePath;
    m_metadataReady = false;
    m_albumArt = QPixmap();

    // Set default values from filename
    QString title = fileInfo.completeBaseName();
    QString artist = "Unknown Artist";
    QString album = "Unknown Album";
    qint64 duration = 0;

    // Load the media file
    m_player->setSource(QUrl::fromLocalFile(filePath));

    // Wait for metadata to be loaded (with timeout)
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(3000); // 3 seconds timeout

    connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(this, &MetadataExtractor::destroyed, &loop, &QEventLoop::quit);

    // Connect signals to exit event loop
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia ||
            status == QMediaPlayer::InvalidMedia ||
            status == QMediaPlayer::NoMedia) {
            m_metadataReady = true;
            loop.quit();
        }
    });

    timeoutTimer.start();
    loop.exec();

    // Extract metadata if loaded successfully
    if (m_metadataReady && m_player->mediaStatus() == QMediaPlayer::LoadedMedia) {
        extractMetadataFromPlayer();

        // Get title
        QString metaTitle = m_player->metaData().stringValue(QMediaMetaData::Title);
        if (!metaTitle.isEmpty()) {
            title = metaTitle;
        }

        // Get artist
        QString metaArtist = m_player->metaData().stringValue(QMediaMetaData::ContributingArtist);
        if (metaArtist.isEmpty()) {
            metaArtist = m_player->metaData().stringValue(QMediaMetaData::AlbumArtist);
        }
        if (!metaArtist.isEmpty()) {
            artist = metaArtist;
        }

        // Get album
        QString metaAlbum = m_player->metaData().stringValue(QMediaMetaData::AlbumTitle);
        if (!metaAlbum.isEmpty()) {
            album = metaAlbum;
        }

        // Get duration
        duration = m_player->duration();
    } else {
        qWarning() << "Failed to load metadata for:" << filePath;
    }

    // Clear the player
    m_player->setSource(QUrl());

    // Create track with extracted metadata
    Track track(filePath, title, artist, album, duration);

    // Set file size and date added
    track.setFileSize(fileInfo.size());
    track.setDateAdded(fileInfo.lastModified());

    // Save album art if extracted
    if (!m_albumArt.isNull()) {
        // IMPORTANT: Set the QPixmap directly for immediate use
        track.setAlbumArt(m_albumArt);

        // Also save to file for future use
        QString albumArtPath = fileInfo.absolutePath() + "/." +
                               fileInfo.completeBaseName() + "_cover.jpg";
        if (m_albumArt.save(albumArtPath, "JPEG", 90)) {
            track.setAlbumArtPath(albumArtPath);
        }
    } else {
        // Look for cover in directory
        QDir dir = fileInfo.dir();
        QStringList imageFilters;
        imageFilters << "cover.jpg" << "cover.png" << "folder.jpg" << "folder.png";

        QStringList imageFiles = dir.entryList(imageFilters, QDir::Files);
        if (!imageFiles.isEmpty()) {
            QString artPath = dir.absoluteFilePath(imageFiles.first());
            track.setAlbumArtPath(artPath);
            // Load and set the pixmap
            QPixmap coverArt(artPath);
            if (!coverArt.isNull()) {
                track.setAlbumArt(coverArt);
            }
        }
    }

    return track;
}

QPixmap MetadataExtractor::extractAlbumArt(const QString &filePath)
{
    Track track = extractMetadata(filePath);
    return m_albumArt;
}

void MetadataExtractor::extractMetadataFromPlayer()
{
    // Extract album art
    QVariant coverArtData = m_player->metaData().value(QMediaMetaData::CoverArtImage);

    if (coverArtData.isValid()) {
        if (coverArtData.canConvert<QImage>()) {
            QImage image = coverArtData.value<QImage>();
            if (!image.isNull()) {
                m_albumArt = QPixmap::fromImage(image);
                qDebug() << "Album art extracted:" << m_albumArt.size();
            }
        } else if (coverArtData.canConvert<QPixmap>()) {
            m_albumArt = coverArtData.value<QPixmap>();
            qDebug() << "Album art extracted:" << m_albumArt.size();
        }
    }

    // Try alternative cover art key
    if (m_albumArt.isNull()) {
        QVariant thumbData = m_player->metaData().value(QMediaMetaData::ThumbnailImage);
        if (thumbData.isValid() && thumbData.canConvert<QImage>()) {
            QImage image = thumbData.value<QImage>();
            if (!image.isNull()) {
                m_albumArt = QPixmap::fromImage(image);
                qDebug() << "Thumbnail extracted:" << m_albumArt.size();
            }
        }
    }
}

void MetadataExtractor::onMetadataChanged()
{
    qDebug() << "Metadata changed for:" << m_currentFilePath;
}

void MetadataExtractor::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "Media status changed:" << status << "for" << m_currentFilePath;
}

void MetadataExtractor::onErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qWarning() << "Media player error:" << error << errorString << "for" << m_currentFilePath;
}
