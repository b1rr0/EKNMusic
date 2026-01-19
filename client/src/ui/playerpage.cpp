#include "playerpage.h"
#include <QPixmap>
#include <QFile>
#include <QStackedWidget>
#include <QDebug>

PlayerPage::PlayerPage(QWidget *parent)
    : QWidget(parent)
    , isPlaying(false)
    , isShuffle(false)
    , isRepeat(false)
    , isLiked(false)
    , isSeekingByUser(false)
    , playerService(PlayerService::instance())
{
    setupUI();
    applyStyles();
    setupPlayerConnections();
}

PlayerPage::~PlayerPage()
{
}

void PlayerPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Back button at the top left
    backButton = new QPushButton("← Back");
    backButton->setFixedSize(100, 40);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #666666;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 20px;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "   border: 1px solid #000000;"
        "}"
    );
    connect(backButton, &QPushButton::clicked, this, &PlayerPage::onBackClicked);
    mainLayout->addWidget(backButton, 0, Qt::AlignLeft);

    // Album Art (large)
    albumArtLabel = new QLabel();
    albumArtLabel->setFixedSize(400, 400);
    albumArtLabel->setAlignment(Qt::AlignCenter);
    albumArtLabel->setScaledContents(true);
    albumArtLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 8px;"
    );
    mainLayout->addWidget(albumArtLabel, 0, Qt::AlignHCenter);

    // Song Title
    songTitleLabel = new QLabel("No song playing");
    songTitleLabel->setAlignment(Qt::AlignCenter);
    songTitleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #000000;");
    mainLayout->addWidget(songTitleLabel);

    // Artist Name
    songArtistLabel = new QLabel("Select a song to play");
    songArtistLabel->setAlignment(Qt::AlignCenter);
    songArtistLabel->setStyleSheet("font-size: 18px; color: #666666;");
    mainLayout->addWidget(songArtistLabel);

    mainLayout->addSpacing(20);

    // Progress Bar with Time Labels
    progressWidget = new QWidget();
    progressLayout = new QHBoxLayout(progressWidget);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressLayout->setSpacing(15);

    currentTimeLabel = new QLabel("0:00");
    currentTimeLabel->setStyleSheet("font-size: 14px; color: #666666;");

    progressSlider = new QSlider(Qt::Horizontal);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);
    progressSlider->setMinimumWidth(500);

    totalTimeLabel = new QLabel("0:00");
    totalTimeLabel->setStyleSheet("font-size: 14px; color: #666666;");

    progressLayout->addWidget(currentTimeLabel);
    progressLayout->addWidget(progressSlider, 1);
    progressLayout->addWidget(totalTimeLabel);

    mainLayout->addWidget(progressWidget, 0, Qt::AlignHCenter);

    // Control Buttons
    controlsWidget = new QWidget();
    controlButtonsLayout = new QHBoxLayout(controlsWidget);
    controlButtonsLayout->setContentsMargins(0, 0, 0, 0);
    controlButtonsLayout->setSpacing(25);
    controlButtonsLayout->setAlignment(Qt::AlignCenter);

    shuffleButton = createControlButton("🔀", 40);
    previousButton = createControlButton("⏮", 50);
    playPauseButton = createControlButton("▶", 60);
    nextButton = createControlButton("⏭", 50);
    repeatButton = createControlButton("🔁", 40);

    connect(shuffleButton, &QPushButton::clicked, this, &PlayerPage::onShuffleClicked);
    connect(previousButton, &QPushButton::clicked, this, &PlayerPage::onPreviousClicked);
    connect(playPauseButton, &QPushButton::clicked, this, &PlayerPage::onPlayPauseClicked);
    connect(nextButton, &QPushButton::clicked, this, &PlayerPage::onNextClicked);
    connect(repeatButton, &QPushButton::clicked, this, &PlayerPage::onRepeatClicked);

    controlButtonsLayout->addWidget(shuffleButton);
    controlButtonsLayout->addWidget(previousButton);
    controlButtonsLayout->addWidget(playPauseButton);
    controlButtonsLayout->addWidget(nextButton);
    controlButtonsLayout->addWidget(repeatButton);

    mainLayout->addWidget(controlsWidget, 0, Qt::AlignHCenter);

    // Additional Controls (Like + Volume)
    additionalControlsWidget = new QWidget();
    additionalControlsWidget->setMinimumWidth(500);
    additionalControlsLayout = new QHBoxLayout(additionalControlsWidget);
    additionalControlsLayout->setContentsMargins(0, 0, 0, 0);
    additionalControlsLayout->setSpacing(15);

    likeButton = createControlButton("♡", 35);
    connect(likeButton, &QPushButton::clicked, this, &PlayerPage::onLikeClicked);

    additionalControlsLayout->addWidget(likeButton);
    additionalControlsLayout->addStretch();

    // Volume controls
    volumeButton = createControlButton("🔊", 35);
    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(70);
    volumeSlider->setFixedWidth(120);
    connect(volumeSlider, &QSlider::valueChanged, this, &PlayerPage::onVolumeChanged);

    additionalControlsLayout->addWidget(volumeButton);
    additionalControlsLayout->addWidget(volumeSlider);

    mainLayout->addWidget(additionalControlsWidget, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
}

void PlayerPage::applyStyles()
{
    setStyleSheet(
        "QWidget {"
        "   background-color: #FFFFFF;"
        "}"
        "QPushButton {"
        "   background-color: white;"
        "   color: #666666;"
        "   border: 2px solid #d0d0d0;"
        "   border-radius: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "   border: 2px solid #000000;"
        "}"
        "QPushButton#playPauseButton {"
        "   background-color: #000000;"
        "   color: #FFFFFF;"
        "   border: none;"
        "}"
        "QPushButton#playPauseButton:hover {"
        "   background-color: #333333;"
        "}"
        "QPushButton[active=\"true\"] {"
        "   color: #000000;"
        "   border: 2px solid #000000;"
        "   background-color: #f0f0f0;"
        "}"
        "QSlider::groove:horizontal {"
        "   background: #e0e0e0;"
        "   height: 6px;"
        "   border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: #000000;"
        "   width: 16px;"
        "   height: 16px;"
        "   margin: -5px 0;"
        "   border-radius: 8px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "   background: #333333;"
        "}"
        "QSlider::sub-page:horizontal {"
        "   background: #666666;"
        "   border-radius: 3px;"
        "}"
    );

    playPauseButton->setObjectName("playPauseButton");
}

QPushButton* PlayerPage::createControlButton(const QString &icon, int size)
{
    QPushButton *button = new QPushButton(icon);
    button->setFixedSize(size, size);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

QString PlayerPage::formatTime(qint64 milliseconds) const
{
    qint64 seconds = milliseconds / 1000;
    qint64 minutes = seconds / 60;
    seconds = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

void PlayerPage::setupPlayerConnections()
{
    // Connect to PlayerService signals
    connect(playerService, &PlayerService::trackChanged,
            this, &PlayerPage::onTrackChanged);
    connect(playerService, &PlayerService::playbackStateChanged,
            this, &PlayerPage::onPlaybackStateChanged);
    connect(playerService, &PlayerService::positionChanged,
            this, &PlayerPage::onPositionChanged);
    connect(playerService, &PlayerService::durationChanged,
            this, &PlayerPage::onDurationChanged);

    // Track user seeking
    connect(progressSlider, &QSlider::sliderPressed, this, [this]() {
        isSeekingByUser = true;
    });
    connect(progressSlider, &QSlider::sliderReleased, this, [this]() {
        isSeekingByUser = false;
        playerService->seek(progressSlider->value());
    });

    // Initialize volume from player service
    volumeSlider->setValue(playerService->volume());

    // IMPORTANT: Load current track if it exists when player page opens
    Track currentTrack = playerService->currentTrack();
    if (currentTrack.isValid()) {
        onTrackChanged(currentTrack);
    }
}

void PlayerPage::onPlayPauseClicked()
{
    playerService->togglePlayPause();
}

void PlayerPage::onPreviousClicked()
{
    playerService->previous();
}

void PlayerPage::onNextClicked()
{
    playerService->next();
}

void PlayerPage::onShuffleClicked()
{
    isShuffle = !isShuffle;
    shuffleButton->setProperty("active", isShuffle);
    shuffleButton->style()->unpolish(shuffleButton);
    shuffleButton->style()->polish(shuffleButton);

    // Update player service mode
    if (isShuffle) {
        playerService->setPlaybackMode(PlayerService::Shuffle);
    } else if (isRepeat) {
        playerService->setPlaybackMode(PlayerService::RepeatAll);
    } else {
        playerService->setPlaybackMode(PlayerService::Sequential);
    }
}

void PlayerPage::onRepeatClicked()
{
    isRepeat = !isRepeat;
    repeatButton->setProperty("active", isRepeat);
    repeatButton->style()->unpolish(repeatButton);
    repeatButton->style()->polish(repeatButton);

    // Update player service mode
    if (isShuffle) {
        playerService->setPlaybackMode(PlayerService::Shuffle);
    } else if (isRepeat) {
        playerService->setPlaybackMode(PlayerService::RepeatAll);
    } else {
        playerService->setPlaybackMode(PlayerService::Sequential);
    }
}

void PlayerPage::onLikeClicked()
{
    isLiked = !isLiked;
    likeButton->setText(isLiked ? "♥" : "♡");
    // TODO: Persist liked state to backend
}

void PlayerPage::onProgressChanged(int value)
{
    if (!isSeekingByUser) {
        currentTimeLabel->setText(formatTime(value));
    }
}

void PlayerPage::onVolumeChanged(int value)
{
    // Update volume icon based on level
    if (value == 0) {
        volumeButton->setText("🔇");
    } else if (value < 50) {
        volumeButton->setText("🔉");
    } else {
        volumeButton->setText("🔊");
    }

    // Update player service volume
    playerService->setVolume(value);
}

void PlayerPage::onTrackChanged(const Track &track)
{
    qDebug() << "[PlayerPage] Track changed - Title:" << track.title() << "Artist:" << track.artist();

    songTitleLabel->setText(track.title());
    songArtistLabel->setText(track.artist());

    // Load album art if available
    // First try to use the QPixmap directly from track
    if (!track.albumArt().isNull()) {
        qDebug() << "[PlayerPage] Loading album art from QPixmap";
        albumArtLabel->setPixmap(track.albumArt().scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug() << "[PlayerPage] Album art loaded successfully from QPixmap";
    }
    // Fallback to loading from file path
    else if (!track.albumArtPath().isEmpty() && QFile::exists(track.albumArtPath())) {
        qDebug() << "[PlayerPage] Loading album art from:" << track.albumArtPath();
        QPixmap albumArt(track.albumArtPath());
        if (!albumArt.isNull()) {
            albumArtLabel->setPixmap(albumArt.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            qDebug() << "[PlayerPage] Album art loaded successfully from file";
        }
    } else {
        qDebug() << "[PlayerPage] No album art available";
        // Set default placeholder
        albumArtLabel->clear();
        albumArtLabel->setStyleSheet(
            "background-color: #e0e0e0;"
            "border: 1px solid #d0d0d0;"
            "border-radius: 8px;"
        );
    }
}

void PlayerPage::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    isPlaying = (state == QMediaPlayer::PlayingState);
    playPauseButton->setText(isPlaying ? "⏸" : "▶");
}

void PlayerPage::onPositionChanged(qint64 position)
{
    if (!isSeekingByUser) {
        progressSlider->setValue(position);
        currentTimeLabel->setText(formatTime(position));
    }
}

void PlayerPage::onDurationChanged(qint64 duration)
{
    progressSlider->setRange(0, duration);
    totalTimeLabel->setText(formatTime(duration));
}

void PlayerPage::onBackClicked()
{
    // Find the stacked widget and go back to previous page
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget*>(parentWidget());
    if (stackedWidget && stackedWidget->count() > 1) {
        // Go to the first page (usually LikedSongs or the main page)
        stackedWidget->setCurrentIndex(0);
    }
}
