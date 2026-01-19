#include "playlistdata.h"
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

// TrackData serialization
QJsonObject TrackData::toJson() const
{
    QJsonObject obj;
    obj["filePath"] = filePath;
    obj["title"] = title;
    obj["artist"] = artist;
    obj["album"] = album;
    obj["duration"] = duration;
    obj["orderIndex"] = orderIndex;
    obj["dateAdded"] = dateAdded.toString(Qt::ISODate);
    obj["fileSize"] = fileSize;
    return obj;
}

TrackData TrackData::fromJson(const QJsonObject &json)
{
    TrackData data;
    data.filePath = json["filePath"].toString();
    data.title = json["title"].toString();
    data.artist = json["artist"].toString();
    data.album = json["album"].toString();
    data.duration = json["duration"].toInteger();
    data.orderIndex = json["orderIndex"].toInt();
    data.dateAdded = QDateTime::fromString(json["dateAdded"].toString(), Qt::ISODate);
    data.fileSize = json["fileSize"].toVariant().toLongLong();
    return data;
}

// PlaylistData implementation
PlaylistData::PlaylistData()
{
}

void PlaylistData::setTrackData(const QString &filePath, const TrackData &data)
{
    m_tracks[filePath] = data;
}

TrackData PlaylistData::getTrackData(const QString &filePath) const
{
    return m_tracks.value(filePath);
}

bool PlaylistData::hasTrackData(const QString &filePath) const
{
    return m_tracks.contains(filePath);
}

QList<TrackData> PlaylistData::getAllTracksOrdered() const
{
    QList<TrackData> tracks = m_tracks.values();

    // Sort by order index
    std::sort(tracks.begin(), tracks.end(), [](const TrackData &a, const TrackData &b) {
        return a.orderIndex < b.orderIndex;
    });

    return tracks;
}

void PlaylistData::updateOrder(const QList<QString> &orderedFilePaths)
{
    qDebug() << "PlaylistData::updateOrder called with" << orderedFilePaths.size() << "tracks";
    qDebug() << "Current m_tracks size:" << m_tracks.size();

    for (int i = 0; i < orderedFilePaths.size(); ++i) {
        const QString &path = orderedFilePaths[i];
        if (m_tracks.contains(path)) {
            m_tracks[path].orderIndex = i;
            qDebug() << "  Updated order for:" << path << "-> index" << i;
        } else {
            qDebug() << "  WARNING: Track not found in m_tracks:" << path;
        }
    }

    qDebug() << "After update, m_tracks contains" << m_tracks.size() << "tracks";
}

void PlaylistData::removeTrack(const QString &filePath)
{
    m_tracks.remove(filePath);
}

QJsonObject PlaylistData::toJson() const
{
    QJsonObject obj;
    QJsonArray tracksArray;

    for (const TrackData &track : m_tracks.values()) {
        tracksArray.append(track.toJson());
    }

    obj["version"] = 1;
    obj["tracks"] = tracksArray;
    return obj;
}

void PlaylistData::fromJson(const QJsonObject &json)
{
    m_tracks.clear();

    QJsonArray tracksArray = json["tracks"].toArray();
    for (const QJsonValue &value : tracksArray) {
        TrackData track = TrackData::fromJson(value.toObject());
        m_tracks[track.filePath] = track;
    }
}

bool PlaylistData::saveToFile(const QString &filePath) const
{
    qDebug() << "PlaylistData::saveToFile - Saving" << m_tracks.size() << "tracks to:" << filePath;

    QJsonObject obj = toJson();
    QJsonDocument doc(obj);

    // Debug: print what we're saving
    QJsonArray tracksArray = obj["tracks"].toArray();
    for (int i = 0; i < tracksArray.size(); ++i) {
        QJsonObject trackObj = tracksArray[i].toObject();
        qDebug() << "  Track" << i << ":" << trackObj["title"].toString()
                 << "orderIndex:" << trackObj["orderIndex"].toInt();
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    qint64 bytesWritten = file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Playlist data saved successfully!" << bytesWritten << "bytes written";
    return true;
}

bool PlaylistData::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Playlist data file not found, starting fresh:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON in playlist data file:" << filePath;
        return false;
    }

    fromJson(doc.object());
    qDebug() << "Playlist data loaded from:" << filePath << "with" << m_tracks.size() << "tracks";
    return true;
}
