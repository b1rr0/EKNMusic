#include "mediastatemanager.h"
#include <QDebug>

MediaStateManager* MediaStateManager::s_instance = nullptr;

MediaStateManager::MediaStateManager(QObject *parent)
    : QObject(parent)
    , m_activeSource(MediaSource::None)
{
    qDebug() << "MediaStateManager initialized";
}

MediaStateManager::~MediaStateManager()
{
}

MediaStateManager* MediaStateManager::instance()
{
    if (!s_instance) {
        s_instance = new MediaStateManager();
    }
    return s_instance;
}

void MediaStateManager::requestPlayback(MediaSource source)
{
    if (m_activeSource == source) {
        // Already active, nothing to do
        return;
    }

    qDebug() << "Media playback requested for source:" << static_cast<int>(source);

    // Stop current active source
    if (m_activeSource == MediaSource::MusicPlayer) {
        qDebug() << "Stopping music player";
        emit stopMusicPlayer();
    } else if (m_activeSource == MediaSource::RadioStream) {
        qDebug() << "Stopping radio stream";
        emit stopRadio();
    }

    // Set new active source
    m_activeSource = source;
    emit activeSourceChanged(source);

    qDebug() << "Active media source changed to:" << static_cast<int>(source);
}

void MediaStateManager::notifyStopped(MediaSource source)
{
    if (m_activeSource == source) {
        qDebug() << "Media source stopped:" << static_cast<int>(source);
        m_activeSource = MediaSource::None;
        emit activeSourceChanged(MediaSource::None);
    }
}
