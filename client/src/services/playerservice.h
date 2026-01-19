#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QList>
#include "models/track.h"

class PlayerService : public QObject
{
    Q_OBJECT

public:
    enum PlaybackMode {
        Sequential,
        Shuffle,
        RepeatOne,
        RepeatAll
    };

    static PlayerService* instance();

    // Playback controls
    void play();
    void pause();
    void stop();
    void togglePlayPause();
    void next();
    void previous();
    void seek(qint64 position);

    // Volume controls
    void setVolume(int volume); // 0-100
    int volume() const;
    void setMuted(bool muted);
    bool isMuted() const;

    // Track management
    void playTrack(const Track &track);
    void setPlaylist(const QList<Track> &tracks);
    void addToPlaylist(const Track &track);
    void clearPlaylist();

    // State getters
    Track currentTrack() const { return m_currentTrack; }
    QMediaPlayer::PlaybackState playbackState() const;
    qint64 position() const;
    qint64 duration() const;
    bool isPlaying() const;

    // Playback mode
    void setPlaybackMode(PlaybackMode mode);
    PlaybackMode playbackMode() const { return m_playbackMode; }

signals:
    void trackChanged(const Track &track);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeChanged(int volume);
    void mutedChanged(bool muted);
    void playbackModeChanged(PlaybackMode mode);

private:
    explicit PlayerService(QObject *parent = nullptr);
    ~PlayerService();
    PlayerService(const PlayerService&) = delete;
    PlayerService& operator=(const PlayerService&) = delete;

    void setupConnections();
    int getNextTrackIndex();
    int getPreviousTrackIndex();

    static PlayerService *s_instance;

    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    Track m_currentTrack;
    QList<Track> m_playlist;
    int m_currentTrackIndex;
    PlaybackMode m_playbackMode;
};

#endif // PLAYERSERVICE_H
