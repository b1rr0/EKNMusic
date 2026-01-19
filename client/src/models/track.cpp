#include "track.h"
#include <QFileInfo>

Track::Track()
    : m_duration(0)
    , m_isLiked(false)
{
}

Track::Track(const QString &filePath, const QString &title,
             const QString &artist, const QString &album, qint64 duration)
    : m_filePath(filePath)
    , m_title(title)
    , m_artist(artist)
    , m_album(album)
    , m_duration(duration)
    , m_isLiked(false)
{
    // If title is empty, use filename
    if (m_title.isEmpty() && !m_filePath.isEmpty()) {
        QFileInfo fileInfo(m_filePath);
        m_title = fileInfo.completeBaseName();
    }

    // Default artist/album if empty
    if (m_artist.isEmpty()) {
        m_artist = "Unknown Artist";
    }
    if (m_album.isEmpty()) {
        m_album = "Unknown Album";
    }
}

QString Track::formattedDuration() const
{
    qint64 seconds = m_duration / 1000;
    qint64 minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}
