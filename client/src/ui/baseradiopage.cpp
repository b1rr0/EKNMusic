#include "baseradiopage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QDesktopServices>
#include <QResizeEvent>

BaseRadioPage::BaseRadioPage(RadioService *radioService, QWidget *parent)
    : QWidget(parent)
    , m_radioService(radioService)
    , networkManager(new QNetworkAccessManager(this))
    , updateTimer(new QTimer(this))
    , currentDuration(0)
    , currentElapsed(0)
    , m_songOrderCounter(1)
{
    setupUI();
    connectSignals();

    // Update progress bar every second
    updateTimer->setInterval(1000);
    connect(updateTimer, &QTimer::timeout, this, &BaseRadioPage::updateProgressBar);
}

BaseRadioPage::~BaseRadioPage()
{
}

void BaseRadioPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Update background and overlay sizes
    if (backgroundLabel) {
        backgroundLabel->setGeometry(0, 0, width(), height());
    }
    if (overlayWidget) {
        overlayWidget->setGeometry(0, 0, width(), height());
    }
    if (stationTitle) {
        stationTitle->setGeometry(0, 30, width(), 50);
    }
}

void BaseRadioPage::setupUI()
{
    // Main widget layout
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    setupBackgroundLayer();

    // Main content layout (left panel + right panel)
    QWidget *contentWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(contentWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    setupLeftPanel(mainLayout);
    setupRightPanel(mainLayout);

    rootLayout->addWidget(contentWidget);
}

void BaseRadioPage::setupBackgroundLayer()
{
    // Background image (full screen)
    backgroundLabel = new QLabel(this);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setStyleSheet("background-color: #1a1a2e;");
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();

    // Dark overlay
    overlayWidget = new QWidget(this);
    overlayWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0.6);");
    overlayWidget->setGeometry(0, 0, width(), height());
    overlayWidget->lower();

    // Station title (centered at top) - will be set later in loadRadioBackground()
    stationTitle = new QLabel("", this);
    stationTitle->setStyleSheet(
        "color: black;"
        "font-size: 35px;"
        "font-weight: bold;"
        "background: transparent;"
        "-webkit-text-stroke: 2px white;"
        "text-shadow: "
        "   -2px -2px 0 white,"
        "    2px -2px 0 white,"
        "   -2px  2px 0 white,"
        "    2px  2px 0 white;"
    );
    stationTitle->setAlignment(Qt::AlignCenter);
    stationTitle->setGeometry(0, 30, width(), 50);
    stationTitle->raise();
}

void BaseRadioPage::setupLeftPanel(QHBoxLayout *mainLayout)
{
    leftPanel = new QWidget(this);

    leftPanel->setFixedSize(355, 500);
    leftPanel->setStyleSheet(
        "background-color: rgba(40, 40, 50, 0.95);"
        "border-radius: 12px;"
    );

    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(20, 15, 20, 15);
    leftLayout->setSpacing(10);

    // Album art
    albumArtLabel = new QLabel(leftPanel);
    albumArtLabel->setFixedSize(200, 200);
    albumArtLabel->setScaledContents(true);
    albumArtLabel->setStyleSheet(
        "background-color: #2a2a3a;"
        "border-radius: 8px;"
        "border: 1px solid #3a3a4a;"
    );
    albumArtLabel->setAlignment(Qt::AlignCenter);

    // Song info
    artistLabel = new QLabel("Artist Name", leftPanel);
    artistLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 13px;"
        "background: transparent;"
    );

    songTitleLabel = new QLabel("Song Title", leftPanel);
    songTitleLabel->setStyleSheet(
        "color: white;"
        "font-size: 16px;"
        "font-weight: 600;"
        "background: transparent;"
    );
    songTitleLabel->setWordWrap(true);

    // Progress bar with time labels
    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->setSpacing(8);

    timeCurrentLabel = new QLabel("00:00", leftPanel);
    timeCurrentLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 11px;"
        "background: transparent;"
    );

    progressBar = new QSlider(Qt::Horizontal, leftPanel);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setEnabled(false);
    progressBar->setFocusPolicy(Qt::NoFocus);
    progressBar->setStyleSheet(
        "QSlider::groove:horizontal {"
        "   border: none;"
        "   height: 5px;"
        "   background: #3a3a4a;"
        "   border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: #4a9eff;"
        "   width: 12px;"
        "   height: 12px;"
        "   margin: -4px 0;"
        "   border-radius: 6px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "   background: #4a9eff;"
        "   border-radius: 2px;"
        "}"
        "QSlider:disabled {"
        "   opacity: 1.0;"
        "}"
    );

    timeTotalLabel = new QLabel("00:00", leftPanel);
    timeTotalLabel->setStyleSheet(
        "color: #a0a0a0;"
        "font-size: 11px;"
        "background: transparent;"
    );

    timeLayout->addWidget(timeCurrentLabel);
    timeLayout->addWidget(progressBar, 1);
    timeLayout->addWidget(timeTotalLabel);

    // Player controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(10);
    controlsLayout->setAlignment(Qt::AlignCenter);

    playPauseBtn = new QPushButton(leftPanel);
    playPauseBtn->setFixedSize(50, 50);
    playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.svg"));
    playPauseBtn->setIconSize(QSize(50, 50));
    playPauseBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
    );
    connect(playPauseBtn, &QPushButton::clicked, this, &BaseRadioPage::onPlayPauseClicked);

    // Volume controls
    volumeBtn = new QPushButton("🔊", leftPanel);
    volumeBtn->setFixedSize(30, 30);
    volumeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: white;"
        "   border: none;"
        "   font-size: 16px;"
        "}"
    );

    volumeSlider = new QSlider(Qt::Horizontal, leftPanel);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(75);
    volumeSlider->setStyleSheet(progressBar->styleSheet());
    connect(volumeSlider, &QSlider::valueChanged, this, &BaseRadioPage::onVolumeSliderChanged);
    connect(volumeBtn, &QPushButton::clicked, this, &BaseRadioPage::onVolumeBtnClicked);

    controlsLayout->addWidget(playPauseBtn);
    controlsLayout->addSpacing(10);
    controlsLayout->addWidget(volumeBtn);
    controlsLayout->addWidget(volumeSlider);

    // Bottom button (REQUEST SONG only)
    QString buttonStyle =
        "QPushButton {"
        "   background-color: rgba(60, 60, 70, 0.9);"
        "   color: white;"
        "   border: 1px solid #4a4a5a;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-size: 12px;"
        "   font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(70, 70, 80, 0.9);"
        "   border: 1px solid #5a5a6a;"
        "}";

    requestSongBtn = new QPushButton("❓ REQUEST SONG", leftPanel);
    requestSongBtn->setStyleSheet(buttonStyle);
    connect(requestSongBtn, &QPushButton::clicked, this, &BaseRadioPage::onRequestSongClicked);

    // Add to left panel layout
    leftLayout->addWidget(albumArtLabel);
    leftLayout->addWidget(artistLabel);
    leftLayout->addWidget(songTitleLabel);
    leftLayout->addLayout(timeLayout);
    leftLayout->addLayout(controlsLayout);
    leftLayout->addStretch();
    leftLayout->addWidget(requestSongBtn);

    mainLayout->addWidget(leftPanel);
}

void BaseRadioPage::setupRightPanel(QHBoxLayout *mainLayout)
{
    rightPanel = new QWidget(this);
    rightPanel->setMaximumHeight(500);
    rightPanel->setStyleSheet(
        "background-color: rgba(40, 40, 50, 0.95);"
        "border-radius: 12px;"
    );

    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(20, 20, 20, 20);
    rightLayout->setSpacing(15);

    // Title
    QLabel *songListTitle = new QLabel("PLAYLIST", rightPanel);
    songListTitle->setStyleSheet(
        "color: white;"
        "font-size: 18px;"
        "font-weight: bold;"
        "background: transparent;"
    );
    songListTitle->setAlignment(Qt::AlignCenter);

    // Song list widget
    songListWidget = new QListWidget(rightPanel);
    songListWidget->setSpacing(0);
    songListWidget->setUniformItemSizes(true);
    songListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    songListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    songListWidget->setStyleSheet(
        "QListWidget { padding: 0; margin: 0; }"
        "QListWidget::viewport { padding: 0; margin: 0; }"
        "QListWidget::item {"
        "   padding: 6px 10px;"
        "   margin: 0;"
        "   border-bottom: 1px solid #3a3a4a;"
        "}"
    );


    rightLayout->addWidget(songListTitle);
    rightLayout->addWidget(songListWidget);

    mainLayout->addWidget(rightPanel, 1);
}

void BaseRadioPage::loadRadioBackground()
{
    // Set station title now that the derived class is fully constructed
    stationTitle->setText(getStationName());

    // Start fetching now playing data
    m_radioService->fetchNowPlaying();

    // Fetch song history and queue
    m_radioService->fetchSongHistory(5);
    m_radioService->fetchQueue();

    // Start the update timer
    updateTimer->start();
}

void BaseRadioPage::connectSignals()
{
    // Connect RadioService signals
    connect(m_radioService, &RadioService::nowPlayingUpdated,
            this, &BaseRadioPage::onNowPlayingUpdated);

    connect(m_radioService, &RadioService::playbackStateChanged,
            this, &BaseRadioPage::onPlaybackStateChanged);

    connect(m_radioService, &RadioService::songHistoryUpdated,
            this, &BaseRadioPage::onSongHistoryReceived);

    connect(m_radioService, &RadioService::queueUpdated,
            this, &BaseRadioPage::onQueueReceived);
}

void BaseRadioPage::onNowPlayingUpdated(const RadioService::NowPlayingInfo &info)
{
    // Check if current song changed
    bool songChanged = (m_previousCurrentSongTitle != info.song.title);

    if (songChanged) {
        qDebug() << "Current song changed to:" << info.song.title;
        m_previousCurrentSongTitle = info.song.title;
        updateSongList();
    }

    // Update song info
    songTitleLabel->setText(info.song.title);
    artistLabel->setText(info.song.artist);
    stationTitle->setText(info.stationName);

    // Update timing
    currentElapsed = info.elapsed;
    currentDuration = info.duration;

    if (currentDuration > 0) {
        int progress = (currentElapsed * 100) / currentDuration;
        progressBar->setValue(progress);

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
    if (!info.song.artUrl.isEmpty() && info.song.artUrl != currentBackgroundUrl) {
        updateBackgroundImage(info.song.artUrl);
    }

    // Update song list
    updateSongList();

    qDebug() << "Now playing updated:" << info.song.artist << "-" << info.song.title;
}

void BaseRadioPage::updateBackgroundImage(const QString &imageUrl)
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
                albumArtLabel->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

                qDebug() << "Background and album art updated";
            }
        }
        imageReply->deleteLater();
    });
}

void BaseRadioPage::onPlaybackStateChanged(bool isPlaying)
{
    if (isPlaying) {
        playPauseBtn->setIcon(QIcon(":/images/src/resources/images/pauseButton.svg"));
    } else {
        playPauseBtn->setIcon(QIcon(":/images/src/resources/images/playButton.svg"));
    }
}

void BaseRadioPage::onPlayPauseClicked()
{
    m_radioService->togglePlayPause();
}

void BaseRadioPage::onVolumeSliderChanged(int value)
{
    m_radioService->setVolume(value);
}

void BaseRadioPage::onVolumeBtnClicked()
{
    bool currentlyMuted = m_radioService->isMuted();
    m_radioService->setMuted(!currentlyMuted);
    volumeBtn->setText(currentlyMuted ? "🔊" : "🔇");
}

void BaseRadioPage::onRequestSongClicked()
{
    qDebug() << "Request Song button clicked - opening URL:" << getRequestSongUrl();
    QDesktopServices::openUrl(QUrl(getRequestSongUrl()));
}

void BaseRadioPage::updateProgressBar()
{
    // Increment elapsed time locally
    if (m_radioService->isPlaying() && currentDuration > 0 && currentElapsed < currentDuration) {
        currentElapsed += 1;

        int progress = (currentElapsed * 100) / currentDuration;
        progressBar->setValue(progress);

        int elapsedMin = currentElapsed / 60;
        int elapsedSec = currentElapsed % 60;
        timeCurrentLabel->setText(QString("%1:%2")
            .arg(elapsedMin, 2, 10, QChar('0'))
            .arg(elapsedSec, 2, 10, QChar('0')));
    }

    // Fetch fresh data when song is near the end (last 10 seconds)
    int timeRemaining = currentDuration - currentElapsed;
    if (timeRemaining <= 10 && timeRemaining > 0) {
        static bool fetchedForCurrentSong = false;
        if (!fetchedForCurrentSong) {
            qDebug() << "Song ending soon (" << timeRemaining << "s remaining), fetching fresh data...";
            m_radioService->fetchNowPlaying();
            m_radioService->fetchSongHistory(5);
            m_radioService->fetchQueue();
            fetchedForCurrentSong = true;
        }
    } else {
        static bool fetchedForCurrentSong = false;
        fetchedForCurrentSong = false;
    }
}

void BaseRadioPage::onSongHistoryReceived(const QList<RadioService::SongInfo> &history)
{
    // Check if history actually changed
    bool historyChanged = false;
    if (m_songHistory.size() != history.size()) {
        historyChanged = true;
    } else {
        for (int i = 0; i < history.size(); i++) {
            if (m_songHistory[i].title != history[i].title || m_songHistory[i].artist != history[i].artist) {
                historyChanged = true;
                break;
            }
        }
    }

    if (historyChanged) {
        m_songHistory = history;
        qDebug() << "Song history changed:" << history.size() << "songs";
        updateSongList();
    }
}

void BaseRadioPage::onQueueReceived(const QList<RadioService::SongInfo> &queue)
{
    // Check if queue actually changed
    bool queueChanged = false;
    if (m_queue.size() != queue.size()) {
        queueChanged = true;
    } else {
        for (int i = 0; i < queue.size(); i++) {
            if (m_queue[i].title != queue[i].title || m_queue[i].artist != queue[i].artist) {
                queueChanged = true;
                break;
            }
        }
    }

    if (queueChanged) {
        m_queue = queue;
        qDebug() << "Queue changed:" << queue.size() << "songs";
        updateSongList();
    }
}

void BaseRadioPage::updateSongList()
{
    songListWidget->clear();
    m_songOrderCounter = 1;  // Reset counter

    qDebug() << "=== UPDATING SONG LIST ===";
    qDebug() << "History size:" << m_songHistory.size();
    qDebug() << "Queue size:" << m_queue.size();

    // Get current song
    RadioService::NowPlayingInfo currentInfo = m_radioService->currentNowPlaying();
    QString currentTitle = currentInfo.song.title;

    qDebug() << "Current song:" << currentInfo.song.artist << "-" << currentTitle;

    // Add previous songs (from history) - max 2, excluding current song
    // History API returns songs in reverse chronological order (newest first)
    // So we take the FIRST items (most recent past songs)
    int historyToShow = 0;

    qDebug() << "Adding up to 2 songs from history (size:" << m_songHistory.size() << ")";
    for (int i = 0; i < m_songHistory.size() && historyToShow < 2; i++) {
        // Skip if this is the current song
        if (m_songHistory[i].title == currentTitle && m_songHistory[i].artist == currentInfo.song.artist) {
            qDebug() << "  Skipping duplicate current song in history:" << m_songHistory[i].title;
            continue;
        }
        qDebug() << "  Adding PREVIOUS [" << i << "]:" << m_songHistory[i].artist << "-" << m_songHistory[i].title;
        addSongToList(m_songHistory[i], "PREVIOUS", false);
        historyToShow++;
    }
    qDebug() << "Total PREVIOUS songs added:" << historyToShow;

    // Add current song
    if (!currentInfo.song.title.isEmpty()) {
        qDebug() << "Adding NOW PLAYING:" << currentInfo.song.title;
        addSongToList(currentInfo.song, "NOW PLAYING", true);
    }

    // Add upcoming songs (from queue) - only 1 NEXT song
    if (!m_queue.isEmpty()) {
        qDebug() << "Adding NEXT song:" << m_queue[0].artist << "-" << m_queue[0].title;
        addSongToList(m_queue[0], "NEXT", false);
    }

    qDebug() << "=== SONG LIST UPDATE COMPLETE - Total items:" << songListWidget->count() << "===";
}

void BaseRadioPage::addSongToList(const RadioService::SongInfo &song, const QString &label, bool isCurrent)
{
    // Skip empty songs
    if (song.title.isEmpty() && song.artist.isEmpty()) {
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(songListWidget);

    // Create custom widget with horizontal layout
    QWidget *itemWidget = new QWidget();
    QHBoxLayout *mainItemLayout = new QHBoxLayout(itemWidget);
    mainItemLayout->setContentsMargins(20, 12, 20, 12);
    mainItemLayout->setSpacing(16);

    // Album art thumbnail (70x70)
    QLabel *thumbnailLabel = new QLabel(itemWidget);
    thumbnailLabel->setFixedSize(70, 70);
    thumbnailLabel->setScaledContents(true);
    thumbnailLabel->setStyleSheet(
        "background-color: #2a2a3a;"
        "border-radius: 4px;"
    );

    // Load thumbnail from song art URL
    if (!song.artUrl.isEmpty()) {
        QNetworkAccessManager *thumbManager = new QNetworkAccessManager(itemWidget);
        QNetworkRequest thumbRequest(song.artUrl);
        QNetworkReply *thumbReply = thumbManager->get(thumbRequest);

        connect(thumbReply, &QNetworkReply::finished, [thumbnailLabel, thumbReply]() {
            if (thumbReply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(thumbReply->readAll());
                if (!pixmap.isNull()) {
                    thumbnailLabel->setPixmap(pixmap.scaled(70, 70, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                }
            }
            thumbReply->deleteLater();
        });
    }

    // Song info (vertical layout)
    QWidget *songInfoWidget = new QWidget(itemWidget);
    QVBoxLayout *songInfoLayout = new QVBoxLayout(songInfoWidget);
    songInfoLayout->setContentsMargins(0, 0, 0, 0);
    songInfoLayout->setSpacing(6);

    // Label (PREVIOUS/NOW PLAYING/NEXT)
    QLabel *labelText = new QLabel(label, songInfoWidget);
    labelText->setStyleSheet(QString(
        "color: %1;"
        "font-size: 12px;"
        "font-weight: bold;"
        "background: transparent;"
        "letter-spacing: 0.5px;"
    ).arg(isCurrent ? "#4a9eff" : "#bbb"));

    // Title
    QLabel *titleText = new QLabel(songInfoWidget);
    titleText->setTextFormat(Qt::PlainText);
    titleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    titleText->setCursor(Qt::IBeamCursor);

    QFont titleFont;
    titleFont.setPixelSize(18);
    titleFont.setWeight(QFont::DemiBold);
    titleText->setFont(titleFont);
    titleText->setStyleSheet("color: white; background: transparent;");

    // Use elided text
    QFontMetrics metrics(titleFont);
    QString elidedTitle = metrics.elidedText(song.title, Qt::ElideRight, 700);
    titleText->setText(elidedTitle);
    titleText->setToolTip(song.title);  // Full text on hover

    // Artist
    QLabel *artistText = new QLabel(songInfoWidget);
    artistText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    artistText->setCursor(Qt::IBeamCursor);

    QFont artistFont;
    artistFont.setPixelSize(15);
    artistText->setFont(artistFont);
    artistText->setStyleSheet("color: #ccc; background: transparent;");

    // Use elided text
    QFontMetrics artistMetrics(artistFont);
    QString elidedArtist = artistMetrics.elidedText(song.artist, Qt::ElideRight, 700);
    artistText->setText(elidedArtist);
    artistText->setToolTip(song.artist);  // Full text on hover

    songInfoLayout->addWidget(labelText);
    songInfoLayout->addWidget(titleText);
    songInfoLayout->addWidget(artistText);

    // Add all to main layout
    mainItemLayout->addWidget(thumbnailLabel);
    mainItemLayout->addWidget(songInfoWidget, 1);

    item->setSizeHint(QSize(itemWidget->width(), 100));

    if (isCurrent) {
        itemWidget->setStyleSheet("background-color: rgba(74, 158, 255, 0.2); border-radius: 4px;");
    }

    songListWidget->addItem(item);
    songListWidget->setItemWidget(item, itemWidget);

    if (isCurrent) {
        songListWidget->setCurrentItem(item);
    }
}
