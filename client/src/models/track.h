#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QUrl>
#include <QPixmap>

class Track
{
public:
    Track();
    Track(const QString &filePath, const QString &title = "",
          const QString &artist = "", const QString &album = "",
          qint64 duration = 0);

    // Getters
    QString filePath() const { return m_filePath; }
    QUrl fileUrl() const { return QUrl::fromLocalFile(m_filePath); }
    QString title() const { return m_title; }
    QString artist() const { return m_artist; }
    QString album() const { return m_album; }
    qint64 duration() const { return m_duration; } // in milliseconds
    bool isLiked() const { return m_isLiked; }
    QString albumArtPath() const { return m_albumArtPath; }
    QPixmap albumArt() const { return m_albumArt; }

    // Setters
    void setFilePath(const QString &path) { m_filePath = path; }
    void setTitle(const QString &title) { m_title = title; }
    void setArtist(const QString &artist) { m_artist = artist; }
    void setAlbum(const QString &album) { m_album = album; }
    void setDuration(qint64 duration) { m_duration = duration; }
    void setLiked(bool liked) { m_isLiked = liked; }
    void setAlbumArtPath(const QString &path) { m_albumArtPath = path; }
    void setAlbumArt(const QPixmap &pixmap) { m_albumArt = pixmap; }

    // Helper methods
    QString formattedDuration() const;
    bool isValid() const { return !m_filePath.isEmpty(); }

private:
    QString m_filePath;
    QString m_title;
    QString m_artist;
    QString m_album;
    qint64 m_duration; // in milliseconds
    bool m_isLiked;
    QString m_albumArtPath;
    QPixmap m_albumArt;
};

#endif // TRACK_H
