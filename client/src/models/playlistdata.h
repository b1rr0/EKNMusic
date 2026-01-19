#ifndef PLAYLISTDATA_H
#define PLAYLISTDATA_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

struct TrackData
{
    QString filePath;
    QString title;
    QString artist;
    QString album;
    qint64 duration;
    int orderIndex; // Position in the playlist
    QDateTime dateAdded;
    qint64 fileSize; // in bytes

    // Serialization
    QJsonObject toJson() const;
    static TrackData fromJson(const QJsonObject &json);
};

class PlaylistData
{
public:
    PlaylistData();

    // Add or update track data
    void setTrackData(const QString &filePath, const TrackData &data);

    // Get track data
    TrackData getTrackData(const QString &filePath) const;
    bool hasTrackData(const QString &filePath) const;

    // Get all tracks sorted by order index
    QList<TrackData> getAllTracksOrdered() const;

    // Update order for multiple tracks
    void updateOrder(const QList<QString> &orderedFilePaths);

    // Remove track data
    void removeTrack(const QString &filePath);

    // Serialization
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    // Save/Load from file
    bool saveToFile(const QString &filePath) const;
    bool loadFromFile(const QString &filePath);

private:
    QMap<QString, TrackData> m_tracks; // filePath -> TrackData
};

#endif // PLAYLISTDATA_H
