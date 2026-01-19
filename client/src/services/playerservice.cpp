#include "playerservice.h"
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QDir>

PlayerService* PlayerService::s_instance = nullptr;

PlayerService::PlayerService(QObject *parent)
    : QObject(parent)
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_currentTrackIndex(-1)
    , m_playbackMode(Sequential)
{
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.7); // Default volume 70%
    setupConnections();
}

PlayerService::~PlayerService()
{
}

PlayerService* PlayerService::instance()
{
    if (!s_instance) {
        s_instance = new PlayerService();
    }
    return s_instance;
}

void PlayerService::setupConnections()
{
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged,
            this, &PlayerService::playbackStateChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
            this, &PlayerService::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged,
            this, &PlayerService::durationChanged);

    // Auto-play next track when current track finishes
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
            this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_playbackMode == RepeatOne) {
                m_mediaPlayer->setPosition(0);
                m_mediaPlayer->play();
            } else {
                next();
            }
        }
    });
}

void PlayerService::play()
{
    if (m_currentTrack.isValid()) {
        m_mediaPlayer->play();
    }
}

void PlayerService::pause()
{
    m_mediaPlayer->pause();
}

void PlayerService::stop()
{
    m_mediaPlayer->stop();
}

void PlayerService::togglePlayPause()
{
    if (isPlaying()) {
        pause();
    } else {
        play();
    }
}

void PlayerService::next()
{
    if (m_playlist.isEmpty()) {
        return;
    }

    int nextIndex = getNextTrackIndex();
    if (nextIndex >= 0 && nextIndex < m_playlist.size()) {
        m_currentTrackIndex = nextIndex;
        playTrack(m_playlist[m_currentTrackIndex]);
    }
}

void PlayerService::previous()
{
    if (m_playlist.isEmpty()) {
        return;
    }

    // If we're more than 3 seconds into the song, restart it
    if (position() > 3000) {
        seek(0);
        return;
    }

    int prevIndex = getPreviousTrackIndex();
    if (prevIndex >= 0 && prevIndex < m_playlist.size()) {
        m_currentTrackIndex = prevIndex;
        playTrack(m_playlist[m_currentTrackIndex]);
    }
}

void PlayerService::seek(qint64 position)
{
    m_mediaPlayer->setPosition(position);
}

void PlayerService::setVolume(int volume)
{
    qreal normalizedVolume = qBound(0, volume, 100) / 100.0;
    m_audioOutput->setVolume(normalizedVolume);
    emit volumeChanged(volume);
}

int PlayerService::volume() const
{
    return static_cast<int>(m_audioOutput->volume() * 100);
}

void PlayerService::setMuted(bool muted)
{
    m_audioOutput->setMuted(muted);
    emit mutedChanged(muted);
}

bool PlayerService::isMuted() const
{
    return m_audioOutput->isMuted();
}

void PlayerService::playTrack(const Track &track)
{
    if (!track.isValid()) {
        return;
    }

    m_currentTrack = track;
    m_mediaPlayer->setSource(track.fileUrl());
    m_mediaPlayer->play();
    emit trackChanged(track);
}

void PlayerService::setPlaylist(const QList<Track> &tracks)
{
    m_playlist = tracks;
    m_currentTrackIndex = -1;

    if (!m_playlist.isEmpty()) {
        m_currentTrackIndex = 0;
        playTrack(m_playlist[0]);
    }
}

void PlayerService::addToPlaylist(const Track &track)
{
    m_playlist.append(track);
}

void PlayerService::clearPlaylist()
{
    m_playlist.clear();
    m_currentTrackIndex = -1;
    stop();
}

QMediaPlayer::PlaybackState PlayerService::playbackState() const
{
    return m_mediaPlayer->playbackState();
}

qint64 PlayerService::position() const
{
    return m_mediaPlayer->position();
}

qint64 PlayerService::duration() const
{
    return m_mediaPlayer->duration();
}

bool PlayerService::isPlaying() const
{
    return m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState;
}

void PlayerService::setPlaybackMode(PlaybackMode mode)
{
    m_playbackMode = mode;
    emit playbackModeChanged(mode);
}

int PlayerService::getNextTrackIndex()
{
    if (m_playlist.isEmpty()) {
        return -1;
    }

    if (m_playbackMode == Shuffle) {
        // Random track (avoid playing the same track)
        int randomIndex = QRandomGenerator::global()->bounded(m_playlist.size());
        if (m_playlist.size() > 1) {
            while (randomIndex == m_currentTrackIndex) {
                randomIndex = QRandomGenerator::global()->bounded(m_playlist.size());
            }
        }
        return randomIndex;
    } else {
        // Sequential or repeat
        int nextIndex = m_currentTrackIndex + 1;
        if (nextIndex >= m_playlist.size()) {
            if (m_playbackMode == RepeatAll) {
                return 0; // Loop to beginning
            }
            return -1; // End of playlist
        }
        return nextIndex;
    }
}

int PlayerService::getPreviousTrackIndex()
{
    if (m_playlist.isEmpty()) {
        return -1;
    }

    if (m_playbackMode == Shuffle) {
        // For simplicity, just go to previous in list
        int prevIndex = m_currentTrackIndex - 1;
        if (prevIndex < 0) {
            return m_playlist.size() - 1;
        }
        return prevIndex;
    } else {
        int prevIndex = m_currentTrackIndex - 1;
        if (prevIndex < 0) {
            if (m_playbackMode == RepeatAll) {
                return m_playlist.size() - 1; // Loop to end
            }
            return 0; // Stay at beginning
        }
        return prevIndex;
    }
}
