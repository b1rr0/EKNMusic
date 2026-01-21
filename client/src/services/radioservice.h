#ifndef RADIOSERVICE_H
#define RADIOSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "mediastatemanager.h"

/**
 * @brief Service for managing radio streaming and AzuraCast API interactions
 *
 * Handles:
 * - Radio stream playback
 * - Now playing information
 * - Song history
 * - Song requests
 * - Queue management
 */
class RadioService : public QObject
{
    Q_OBJECT

public:
    struct SongInfo {
        QString id;
        QString title;
        QString artist;
        QString album;
        QString artUrl;
        int duration = 0;
    };

    struct NowPlayingInfo {
        SongInfo song;
        QString stationName;
        int elapsed = 0;
        int duration = 0;
        int listeners = 0;
        bool isOnline = false;
    };

    static RadioService* instance();

    // Playback controls
    void playRadio();
    void stopRadio();
    void togglePlayPause();
    bool isPlaying() const;

    // Volume controls
    void setVolume(int volume); // 0-100
    int volume() const;
    void setMuted(bool muted);
    bool isMuted() const;

    // API methods
    void fetchNowPlaying();
    void fetchSongHistory(int limit = 10);
    void fetchRequestableSongs(const QString &search = QString());
    void submitSongRequest(const QString &requestId);
    void fetchQueue();

    // Getters
    NowPlayingInfo currentNowPlaying() const { return m_nowPlayingInfo; }
    QList<SongInfo> songHistory() const { return m_songHistory; }
    QList<SongInfo> requestableSongs() const { return m_requestableSongs; }
    QList<SongInfo> queue() const { return m_queue; }

signals:
    void nowPlayingUpdated(const NowPlayingInfo &info);
    void songHistoryUpdated(const QList<SongInfo> &history);
    void requestableSongsUpdated(const QList<SongInfo> &songs);
    void queueUpdated(const QList<SongInfo> &queue);
    void songRequestSubmitted(bool success, const QString &message);

    void playbackStateChanged(bool isPlaying);
    void volumeChanged(int volume);
    void mutedChanged(bool muted);

    void errorOccurred(const QString &error);

private slots:
    void onNowPlayingReceived(QNetworkReply *reply);
    void onSongHistoryReceived(QNetworkReply *reply);
    void onRequestableSongsReceived(QNetworkReply *reply);
    void onSongRequestSubmitted(QNetworkReply *reply);
    void onQueueReceived(QNetworkReply *reply);

private:
    explicit RadioService(QObject *parent = nullptr);
    ~RadioService();
    RadioService(const RadioService&) = delete;
    RadioService& operator=(const RadioService&) = delete;

    void setupConnections();
    QNetworkRequest createRequest(const QString &endpoint);
    QNetworkRequest createAuthenticatedRequest(const QString &endpoint);
    SongInfo parseSongInfo(const QJsonObject &songObj);

    static RadioService *s_instance;

    // Network
    QNetworkAccessManager *m_networkManager;

    // Media playback
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;

    // Configuration
    QString m_baseUrl;
    QString m_apiKey;
    QString m_stationId;
    QString m_streamUrl;

    // Cached data
    NowPlayingInfo m_nowPlayingInfo;
    QList<SongInfo> m_songHistory;
    QList<SongInfo> m_requestableSongs;
    QList<SongInfo> m_queue;
};

#endif // RADIOSERVICE_H
