#ifndef MUSICSTORAGESERVICE_H
#define MUSICSTORAGESERVICE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDir>
#include "models/track.h"

class MusicStorageService : public QObject
{
    Q_OBJECT

public:
    static MusicStorageService* instance();

    // Directory management
    QString musicDirectory() const { return m_musicDirectory; }
    bool ensureMusicDirectoryExists();

    // Track management
    QList<Track> getDownloadedTracks();
    bool saveTrack(const QString &sourceFilePath, const Track &trackInfo);
    bool deleteTrack(const QString &filePath);

    // Metadata
    Track extractMetadataFromFile(const QString &filePath);

signals:
    void tracksChanged();

private:
    explicit MusicStorageService(QObject *parent = nullptr);
    ~MusicStorageService();
    MusicStorageService(const MusicStorageService&) = delete;
    MusicStorageService& operator=(const MusicStorageService&) = delete;

    void initializeMusicDirectory();
    QString getStandardMusicPath();

    static MusicStorageService *s_instance;
    QString m_musicDirectory;
};

#endif // MUSICSTORAGESERVICE_H
