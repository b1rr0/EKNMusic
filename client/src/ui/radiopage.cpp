#include "radiopage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>

RadioPage::RadioPage(QWidget *parent)
    : QWidget(parent)
    , networkManager(new QNetworkAccessManager(this))
    , updateTimer(new QTimer(this))
    , currentDuration(0)
    , currentElapsed(0)
{
    setupUI();

    connect(networkManager, &QNetworkAccessManager::finished,
            this, &RadioPage::onNowPlayingDataReceived);

    connect(updateTimer, &QTimer::timeout,
            this, &RadioPage::updateNowPlaying);

    // Update every 5 seconds
    updateTimer->setInterval(5000);
}

RadioPage::~RadioPage()
{
}

void RadioPage::setupUI()
{
    // Main widget uses stacked layout for background + overlay
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Background image (full screen)
    backgroundLabel = new QLabel(this);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setStyleSheet("background-color: #1a1a2e;");
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Dark overlay
    overlayWidget = new QWidget(this);
    overlayWidget->setStyleSheet(
        "background-color: rgba(0, 0, 0, 0.6);"
    );
    overlayWidget->setGeometry(0, 0, width(), height());

    // Center container
    QWidget *centerContainer = new QWidget(this);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerContainer);
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->setSpacing(20);

    // Station title
    stationTitle = new QLabel("EKNM Intercom", centerContainer);
    stationTitle->setStyleSheet(
        "color: white;"
        "font-size: 32px;"
        "font-weight: bold;"
        "background: transparent;"
    );
    stationTitle->setAlignment(Qt::AlignCenter);

    // Player card (dark semi-transparent)
    playerCard = new QWidget(centerContainer);
    playerCard->setFixedSize(600, 280);
    playerCard->setStyleSheet(
        "background-color: rgba(40, 40, 50, 0.95);"
        "border-radius: 12px;"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(playerCard);
    cardLayout->setContentsMargins(30, 25, 30, 25);
    cardLayout->setSpacing(15);

    // Album art + song info horizontal layout
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(20);

    // Album art
    albumArtLabel = new QLabel(playerCard);
    albumArtLabel->setFixedSize(120, 120);
    albumArtLabel->setScaledContents(true);
    albumArtLabel->setStyleSheet(
        "background-color: #2a2a3a;"
        "border-radius: 8px;"
        "border: 1px solid #3a3a4a;"
    );

    // Song info (right side)
    QWidget *songInfoWidget = new QWidget(playerCard);
    QVBoxLayout *songInfoLayout = new QVBoxLayout(songInfoWidget);
    songInfoLayout->setSpacing(8);
    songInfoLayout->setContentsMargins(0, 0, 0, 0);

    artistLabel = new QLabel("Artist Name", songInfoWidget);
    artistLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 14px;"
        "background: transparent;"
    );

    songTitleLabel = new QLabel("Song Title", songInfoWidget);
    songTitleLabel->setStyleSheet(
        "color: white;"
        "font-size: 22px;"
        "font-weight: 600;"
        "background: transparent;"
    );
    songTitleLabel->setWordWrap(true);

    songInfoLayout->addWidget(artistLabel);
    songInfoLayout->addWidget(songTitleLabel);
    songInfoLayout->addStretch();

    topLayout->addWidget(albumArtLabel);
    topLayout->addWidget(songInfoWidget, 1);

    // Progress bar with time labels
    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->setSpacing(10);

    timeCurrentLabel = new QLabel("02:10", playerCard);
    timeCurrentLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 12px;"
        "background: transparent;"
    );

    progressBar = new QSlider(Qt::Horizontal, playerCard);
    progressBar->setRange(0, 100);
    progressBar->setValue(35);
    progressBar->setStyleSheet(
        "QSlider::groove:horizontal {"
        "   border: none;"
        "   height: 4px;"
        "   background: #3a3a4a;"
        "   border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: #4a9eff;"
        "   width: 14px;"
        "   height: 14px;"
        "   margin: -5px 0;"
        "   border-radius: 7px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "   background: #4a9eff;"
        "   border-radius: 2px;"
        "}"
    );

    timeTotalLabel = new QLabel("04:16", playerCard);
    timeTotalLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 12px;"
        "background: transparent;"
    );

    timeLayout->addWidget(timeCurrentLabel);
    timeLayout->addWidget(progressBar, 1);
    timeLayout->addWidget(timeTotalLabel);

    // Player controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(15);
    controlsLayout->setAlignment(Qt::AlignCenter);

    playPauseBtn = new QPushButton("▶", playerCard);
    playPauseBtn->setFixedSize(50, 50);
    playPauseBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #1a1a2e;"
        "   border: none;"
        "   border-radius: 25px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "}"
    );

    // Volume controls
    volumeBtn = new QPushButton("🔊", playerCard);
    volumeBtn->setFixedSize(32, 32);
    volumeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: white;"
        "   border: none;"
        "   font-size: 16px;"
        "}"
    );

    volumeSlider = new QSlider(Qt::Horizontal, playerCard);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(75);
    volumeSlider->setStyleSheet(progressBar->styleSheet());

    controlsLayout->addWidget(playPauseBtn);
    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(volumeBtn);
    controlsLayout->addWidget(volumeSlider);

    // Add all to card
    cardLayout->addLayout(topLayout);
    cardLayout->addSpacing(10);
    cardLayout->addLayout(timeLayout);
    cardLayout->addLayout(controlsLayout);

    // Bottom buttons (Song History, Request Song, Playlist)
    QWidget *bottomButtonsWidget = new QWidget(centerContainer);
    QHBoxLayout *bottomButtonsLayout = new QHBoxLayout(bottomButtonsWidget);
    bottomButtonsLayout->setSpacing(15);
    bottomButtonsLayout->setContentsMargins(0, 0, 0, 0);

    songHistoryBtn = new QPushButton("🕒 SONG HISTORY", bottomButtonsWidget);
    requestSongBtn = new QPushButton("❓ REQUEST SONG", bottomButtonsWidget);
    playlistBtn = new QPushButton("📃 PLAYLIST", bottomButtonsWidget);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: rgba(60, 60, 70, 0.9);"
        "   color: white;"
        "   border: 1px solid #4a4a5a;"
        "   border-radius: 8px;"
        "   padding: 12px 24px;"
        "   font-size: 13px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(70, 70, 80, 0.9);"
        "   border: 1px solid #5a5a6a;"
        "}";

    songHistoryBtn->setStyleSheet(buttonStyle);
    requestSongBtn->setStyleSheet(buttonStyle);
    playlistBtn->setStyleSheet(buttonStyle);

    bottomButtonsLayout->addWidget(songHistoryBtn);
    bottomButtonsLayout->addWidget(requestSongBtn);
    bottomButtonsLayout->addWidget(playlistBtn);

    // Add to center layout
    centerLayout->addWidget(stationTitle);
    centerLayout->addWidget(playerCard);
    centerLayout->addWidget(bottomButtonsWidget);

    mainLayout->addWidget(centerContainer);
}

void RadioPage::loadRadioBackground()
{
    fetchNowPlayingData();
    updateTimer->start();
}

void RadioPage::fetchNowPlayingData()
{
    QString apiUrl = "https://radio.eknm.in/api/nowplaying/eknm_intercom";

    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::UserAgentHeader, "EKNMusic/1.0");

    networkManager->get(request);

    qDebug() << "Fetching now playing data from:" << apiUrl;
}

void RadioPage::onNowPlayingDataReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject obj = doc.object();

        qDebug() << "API Response:" << doc.toJson(QJsonDocument::Compact);

        if (obj.contains("now_playing")) {
            QJsonObject nowPlaying = obj["now_playing"].toObject();

            if (nowPlaying.contains("song")) {
                QJsonObject song = nowPlaying["song"].toObject();

                // Extract song info
                currentSongTitle = song["title"].toString();
                currentArtist = song["artist"].toString();
                QString artUrl = song["art"].toString();

                // Update UI
                songTitleLabel->setText(currentSongTitle);
                artistLabel->setText(currentArtist);

                // Update elapsed/duration
                if (nowPlaying.contains("elapsed")) {
                    currentElapsed = nowPlaying["elapsed"].toInt();
                }
                if (nowPlaying.contains("duration")) {
                    currentDuration = nowPlaying["duration"].toInt();
                }

                // Update progress
                if (currentDuration > 0) {
                    int progress = (currentElapsed * 100) / currentDuration;
                    progressBar->setValue(progress);

                    // Format time
                    int elapsedMin = currentElapsed / 60;
                    int elapsedSec = currentElapsed % 60;
                    int durationMin = currentDuration / 60;
                    int durationSec = currentDuration % 60;

                    timeCurrentLabel->setText(QString("%1:%2")
                        .arg(elapsedMin, 2, 10, QChar('0'))
                        .arg(elapsedSec, 2, 10, QChar('0')));
                    timeTotalLabel->setText(QString("%1:%2")
                        .arg(durationMin, 2, 10, QChar('0'))
                        .arg(durationSec, 2, 10, QChar('0')));
                }

                // Update background and album art
                if (!artUrl.isEmpty() && artUrl != currentBackgroundUrl) {
                    updateBackgroundImage(artUrl);
                }
            }
        }
    } else {
        qWarning() << "Failed to fetch now playing data:" << reply->errorString();
    }

    reply->deleteLater();
}

void RadioPage::updateBackgroundImage(const QString &imageUrl)
{
    currentBackgroundUrl = imageUrl;

    QNetworkRequest imageRequest;
    imageRequest.setUrl(QUrl(imageUrl));

    QNetworkReply *imageReply = networkManager->get(imageRequest);

    connect(imageReply, &QNetworkReply::finished, this, [this, imageReply]() {
        if (imageReply->error() == QNetworkReply::NoError) {
            QByteArray imageData = imageReply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                // Set background
                backgroundLabel->setPixmap(pixmap);
                backgroundLabel->setGeometry(0, 0, width(), height());

                // Set album art
                albumArtLabel->setPixmap(pixmap.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

                qDebug() << "Background and album art updated";
            }
        }
        imageReply->deleteLater();
    });
}

void RadioPage::updateNowPlaying()
{
    // Increment elapsed time locally
    if (currentDuration > 0 && currentElapsed < currentDuration) {
        currentElapsed += 5; // Add 5 seconds

        int progress = (currentElapsed * 100) / currentDuration;
        progressBar->setValue(progress);

        int elapsedMin = currentElapsed / 60;
        int elapsedSec = currentElapsed % 60;
        timeCurrentLabel->setText(QString("%1:%2")
            .arg(elapsedMin, 2, 10, QChar('0'))
            .arg(elapsedSec, 2, 10, QChar('0')));
    }

    // Fetch fresh data every update
    fetchNowPlayingData();
}
