#include "radioservice.h"
#include "config/appconfig.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QUrl>

RadioService* RadioService::s_instance = nullptr;

RadioService::RadioService(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
{
    // Load configuration
    m_baseUrl = "https://radio.eknm.in";
    m_stationId = "eknm_intercom";
    m_streamUrl = "https://radio.eknm.in/listen/eknm_intercom/radio.mp3";

    // Load API key from config (will be stored securely)
    m_apiKey = AppConfig::instance()->getRadioApiKey();

    // Setup media player
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.75); // Default 75% volume

    setupConnections();

    // Connect to global media state manager
    connect(MediaStateManager::instance(), &MediaStateManager::stopRadio,
            this, &RadioService::stopRadio);

    qDebug() << "RadioService initialized for station:" << m_stationId;
}

RadioService::~RadioService()
{
}

RadioService* RadioService::instance()
{
    if (!s_instance) {
        s_instance = new RadioService();
    }
    return s_instance;
}

void RadioService::setupConnections()
{
    // Media player connections
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        emit playbackStateChanged(state == QMediaPlayer::PlayingState);
    });

    connect(m_mediaPlayer, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error error, const QString &errorString) {
        qWarning() << "Media player error:" << errorString;
        emit errorOccurred(errorString);
    });
}

// ========== Playback Controls ==========

void RadioService::playRadio()
{
    if (m_streamUrl.isEmpty()) {
        emit errorOccurred("Stream URL not configured");
        return;
    }

    // Request playback from media state manager (will stop music player if active)
    MediaStateManager::instance()->requestPlayback(MediaStateManager::MediaSource::RadioStream);

    qDebug() << "Starting radio stream:" << m_streamUrl;
    m_mediaPlayer->setSource(QUrl(m_streamUrl));
    m_mediaPlayer->play();

    // Start fetching now playing info
    fetchNowPlaying();
}

void RadioService::stopRadio()
{
    qDebug() << "Stopping radio stream";
    m_mediaPlayer->stop();

    // Notify media state manager that radio stopped
    MediaStateManager::instance()->notifyStopped(MediaStateManager::MediaSource::RadioStream);
}

void RadioService::togglePlayPause()
{
    if (isPlaying()) {
        stopRadio();
    } else {
        playRadio();
    }
}

bool RadioService::isPlaying() const
{
    return m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState;
}

// ========== Volume Controls ==========

void RadioService::setVolume(int volume)
{
    float normalizedVolume = qBound(0, volume, 100) / 100.0f;
    m_audioOutput->setVolume(normalizedVolume);
    emit volumeChanged(volume);
}

int RadioService::volume() const
{
    return static_cast<int>(m_audioOutput->volume() * 100);
}

void RadioService::setMuted(bool muted)
{
    m_audioOutput->setMuted(muted);
    emit mutedChanged(muted);
}

bool RadioService::isMuted() const
{
    return m_audioOutput->isMuted();
}

// ========== API Methods ==========

QNetworkRequest RadioService::createRequest(const QString &endpoint)
{
    QString url = m_baseUrl + endpoint;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "EKNMusic/1.0");
    return request;
}

QNetworkRequest RadioService::createAuthenticatedRequest(const QString &endpoint)
{
    QNetworkRequest request =   createRequest(endpoint);
    if (!m_apiKey.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());
    }
    return request;
}

void RadioService::fetchNowPlaying()
{
    QString endpoint = QString("/api/nowplaying/%1").arg(m_stationId);
    QNetworkRequest request = createRequest(endpoint);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onNowPlayingReceived(reply);
    });

    qDebug() << "Fetching now playing from:" << request.url();
}

void RadioService::fetchSongHistory(int limit)
{
    QString endpoint = QString("/api/station/%1/history?limit=%2").arg(m_stationId).arg(limit);
    QNetworkRequest request = createAuthenticatedRequest(endpoint);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSongHistoryReceived(reply);
    });
}

void RadioService::fetchRequestableSongs(const QString &search)
{
    QString endpoint = QString("/api/station/%1/requests").arg(m_stationId);
    if (!search.isEmpty()) {
        endpoint += QString("?search=%1").arg(QString(QUrl::toPercentEncoding(search)));
    }

    QNetworkRequest request = createAuthenticatedRequest(endpoint);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onRequestableSongsReceived(reply);
    });
}

void RadioService::submitSongRequest(const QString &requestId)
{
    QString endpoint = QString("/api/station/%1/request/%2").arg(m_stationId, requestId);
    QNetworkRequest request = createAuthenticatedRequest(endpoint);

    QNetworkReply *reply = m_networkManager->post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSongRequestSubmitted(reply);
    });

    qDebug() << "Submitting song request:" << requestId;
}

void RadioService::fetchQueue()
{
    QString endpoint = QString("/api/station/%1/queue").arg(m_stationId);
    QNetworkRequest request = createAuthenticatedRequest(endpoint);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onQueueReceived(reply);
    });
}

// ========== Response Handlers ==========

RadioService::SongInfo RadioService::parseSongInfo(const QJsonObject &songObj)
{
    SongInfo info;
    info.id = songObj["id"].toString();
    info.title = songObj["title"].toString("Unknown");
    info.artist = songObj["artist"].toString("Unknown");
    info.album = songObj["album"].toString();
    info.artUrl = songObj["art"].toString();
    info.duration = songObj["duration"].toInt(0);
    return info;
}

void RadioService::onNowPlayingReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject obj = doc.object();

        NowPlayingInfo info;

        // Parse station info
        QJsonObject station = obj["station"].toObject();
        info.stationName = station["name"].toString("EKNM Intercom");
        info.isOnline = station["is_online"].toBool(false);

        // Parse now playing
        QJsonObject nowPlaying = obj["now_playing"].toObject();
        info.song = parseSongInfo(nowPlaying["song"].toObject());
        info.elapsed = nowPlaying["elapsed"].toInt(0);
        info.duration = nowPlaying["duration"].toInt(0);

        // Parse listeners
        QJsonObject listeners = obj["listeners"].toObject();
        info.listeners = listeners["current"].toInt(0);

        m_nowPlayingInfo = info;
        emit nowPlayingUpdated(info);

        qDebug() << "Now playing:" << info.song.artist << "-" << info.song.title;
    } else {
        qWarning() << "Failed to fetch now playing:" << reply->errorString();
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}

void RadioService::onSongHistoryReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonArray arr = doc.array();

        QList<SongInfo> history;
        for (const QJsonValue &val : arr) {
            QJsonObject item = val.toObject();
            SongInfo song = parseSongInfo(item["song"].toObject());
            history.append(song);
        }

        m_songHistory = history;
        emit songHistoryUpdated(history);

        qDebug() << "Song history updated:" << history.size() << "tracks";
    } else {
        qWarning() << "Failed to fetch song history:" << reply->errorString();
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}

void RadioService::onRequestableSongsReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonArray arr = doc.array();

        QList<SongInfo> songs;
        for (const QJsonValue &val : arr) {
            QJsonObject item = val.toObject();
            SongInfo song = parseSongInfo(item["song"].toObject());
            // Store request_id in the id field for later use
            song.id = item["request_id"].toString();
            songs.append(song);
        }

        m_requestableSongs = songs;
        emit requestableSongsUpdated(songs);

        qDebug() << "Requestable songs updated:" << songs.size() << "songs";
    } else {
        qWarning() << "Failed to fetch requestable songs:" << reply->errorString();
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}

void RadioService::onSongRequestSubmitted(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        emit songRequestSubmitted(true, "Song requested successfully!");
        qDebug() << "Song request submitted successfully";
    } else {
        QString errorMsg = "Failed to request song";

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() == QNetworkReply::ContentNotFoundError || statusCode == 404) {
            errorMsg = "Song not available for requests";
        } else if (statusCode == 429) {
            errorMsg = "Too many requests. Please wait before requesting another song.";
        }

        emit songRequestSubmitted(false, errorMsg);
        qWarning() << "Song request failed:" << reply->errorString();
    }

    reply->deleteLater();
}

void RadioService::onQueueReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonArray arr = doc.array();

        QList<SongInfo> queue;
        for (const QJsonValue &val : arr) {
            QJsonObject item = val.toObject();
            SongInfo song = parseSongInfo(item["song"].toObject());
            queue.append(song);
        }

        m_queue = queue;
        emit queueUpdated(queue);

        qDebug() << "Queue updated:" << queue.size() << "tracks";
    } else {
        qWarning() << "Failed to fetch queue:" << reply->errorString();
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}
