#include "radiopage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

RadioPage::RadioPage(QWidget *parent)
    : QWidget(parent)
    , networkManager(new QNetworkAccessManager(this))
{
    setupUI();

    connect(networkManager, &QNetworkAccessManager::finished,
            this, &RadioPage::onBackgroundDownloaded);
}

RadioPage::~RadioPage()
{
}

void RadioPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Background label (full size)
    backgroundLabel = new QLabel(this);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setAlignment(Qt::AlignCenter);
    backgroundLabel->setStyleSheet("background-color: #1a1a1a;");

    // Title overlay
    titleLabel = new QLabel("EKNM Intercom Radio", this);
    titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 48px;"
        "font-weight: bold;"
        "background: transparent;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(backgroundLabel);
    mainLayout->addWidget(titleLabel);
}

void RadioPage::loadRadioBackground()
{
    fetchBackgroundFromAPI();
}

void RadioPage::fetchBackgroundFromAPI()
{
    // Fetch background from radio.eknm.in API
    QString apiUrl = "https://radio.eknm.in/api/nowplaying/eknm_intercom";

    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::UserAgentHeader, "EKNMusic/1.0");

    networkManager->get(request);

    qDebug() << "Fetching radio background from:" << apiUrl;
}

void RadioPage::onBackgroundDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        // Parse JSON response
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject obj = doc.object();

        // Get background art URL from now_playing.song.art
        if (obj.contains("now_playing")) {
            QJsonObject nowPlaying = obj["now_playing"].toObject();
            if (nowPlaying.contains("song")) {
                QJsonObject song = nowPlaying["song"].toObject();
                if (song.contains("art")) {
                    currentBackgroundUrl = song["art"].toString();

                    qDebug() << "Background URL:" << currentBackgroundUrl;

                    // Download the actual image
                    QNetworkRequest imageRequest;
                    imageRequest.setUrl(QUrl(currentBackgroundUrl));

                    QNetworkReply *imageReply = networkManager->get(imageRequest);

                    // Use lambda to handle image download separately
                    connect(imageReply, &QNetworkReply::finished, this, [this, imageReply]() {
                        if (imageReply->error() == QNetworkReply::NoError) {
                            QByteArray imageData = imageReply->readAll();
                            QPixmap backgroundPixmap;
                            backgroundPixmap.loadFromData(imageData);

                            if (!backgroundPixmap.isNull()) {
                                backgroundLabel->setPixmap(backgroundPixmap);
                                qDebug() << "Background loaded successfully";
                            }
                        } else {
                            qWarning() << "Failed to download background image:" << imageReply->errorString();
                        }
                        imageReply->deleteLater();
                    });
                }
            }
        }
    } else {
        qWarning() << "Failed to fetch radio API:" << reply->errorString();
    }

    reply->deleteLater();
}
