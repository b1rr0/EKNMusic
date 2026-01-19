#ifndef METADATAEXTRACTOR_H
#define METADATAEXTRACTOR_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaMetaData>
#include "models/track.h"

class MetadataExtractor : public QObject
{
    Q_OBJECT

public:
    explicit MetadataExtractor(QObject *parent = nullptr);
    ~MetadataExtractor();

    // Extract metadata from audio file
    Track extractMetadata(const QString &filePath);

    // Extract only album art
    QPixmap extractAlbumArt(const QString &filePath);

private slots:
    void onMetadataChanged();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;

    QString m_currentFilePath;
    Track m_extractedTrack;
    QPixmap m_albumArt;
    bool m_metadataReady;

    void extractMetadataFromPlayer();
};

#endif // METADATAEXTRACTOR_H
