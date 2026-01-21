#ifndef MEDIASTATEMANAGER_H
#define MEDIASTATEMANAGER_H

#include <QObject>

/**
 * @brief Global media state manager to ensure only one audio source plays at a time
 *
 * This singleton manages the exclusive playback state between:
 * - Music player (PlayerService)
 * - Radio stream (RadioService)
 *
 * When one starts playing, the other automatically stops.
 */
class MediaStateManager : public QObject
{
    Q_OBJECT

public:
    enum class MediaSource {
        None,
        MusicPlayer,
        RadioStream
    };

    static MediaStateManager* instance();

    // Get current active source
    MediaSource activeSource() const { return m_activeSource; }

    // Request to play a specific source (will stop other sources)
    void requestPlayback(MediaSource source);

    // Notify that a source has stopped
    void notifyStopped(MediaSource source);

    // Check if a specific source is active
    bool isActive(MediaSource source) const { return m_activeSource == source; }

signals:
    // Emitted when music player should stop (radio is starting)
    void stopMusicPlayer();

    // Emitted when radio should stop (music is starting)
    void stopRadio();

    // Emitted when active source changes
    void activeSourceChanged(MediaSource source);

private:
    explicit MediaStateManager(QObject *parent = nullptr);
    ~MediaStateManager();
    MediaStateManager(const MediaStateManager&) = delete;
    MediaStateManager& operator=(const MediaStateManager&) = delete;

    static MediaStateManager *s_instance;
    MediaSource m_activeSource;
};

#endif // MEDIASTATEMANAGER_H
