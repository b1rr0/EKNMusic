#include "playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , isPlaying(false)
    , isShuffle(false)
    , isRepeat(false)
{
    setupUI();
    applyStyles();
}

PlayerWidget::~PlayerWidget()
{
}

void PlayerWidget::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(20);

    // === Left Section: Song Info ===
    songInfoWidget = new QWidget(this);
    songInfoWidget->setFixedWidth(250);
    songInfoLayout = new QHBoxLayout(songInfoWidget);
    songInfoLayout->setContentsMargins(0, 0, 0, 0);
    songInfoLayout->setSpacing(10);

    // Album art
    albumArtLabel = new QLabel();
    albumArtLabel->setFixedSize(60, 60);
    albumArtLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 4px;"
    );

    // Song title and artist
    QVBoxLayout *songTextLayout = new QVBoxLayout();
    songTitleLabel = new QLabel("No song playing");
    songTitleLabel->setStyleSheet("font-size: 13px; font-weight: 600; color: #000000;");

    songArtistLabel = new QLabel("Select a song to play");
    songArtistLabel->setStyleSheet("font-size: 11px; color: #666666;");

    songTextLayout->addWidget(songTitleLabel);
    songTextLayout->addWidget(songArtistLabel);
    songTextLayout->addStretch();

    // Like button
    likeButton = new QPushButton("♡");
    likeButton->setFixedSize(28, 28);
    likeButton->setCursor(Qt::PointingHandCursor);

    songInfoLayout->addWidget(albumArtLabel);
    songInfoLayout->addLayout(songTextLayout, 1);
    songInfoLayout->addWidget(likeButton);

    // === Center Section: Player Controls ===
    controlsWidget = new QWidget(this);
    controlsLayout = new QVBoxLayout(controlsWidget);
    controlsLayout->setContentsMargins(0, 0, 0, 0);
    controlsLayout->setSpacing(8);

    // Control buttons
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(15);
    buttonsLayout->setAlignment(Qt::AlignCenter);

    shuffleButton = createControlButton("🔀", 24);
    previousButton = createControlButton("⏮", 28);
    playPauseButton = createControlButton("▶", 36);
    nextButton = createControlButton("⏭", 28);
    repeatButton = createControlButton("🔁", 24);

    connect(shuffleButton, &QPushButton::clicked, this, &PlayerWidget::onShuffleClicked);
    connect(previousButton, &QPushButton::clicked, this, &PlayerWidget::onPreviousClicked);
    connect(playPauseButton, &QPushButton::clicked, this, &PlayerWidget::onPlayPauseClicked);
    connect(nextButton, &QPushButton::clicked, this, &PlayerWidget::onNextClicked);
    connect(repeatButton, &QPushButton::clicked, this, &PlayerWidget::onRepeatClicked);

    buttonsLayout->addWidget(shuffleButton);
    buttonsLayout->addWidget(previousButton);
    buttonsLayout->addWidget(playPauseButton);
    buttonsLayout->addWidget(nextButton);
    buttonsLayout->addWidget(repeatButton);

    // Progress bar with time labels
    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->setSpacing(10);

    currentTimeLabel = new QLabel("0:00");
    currentTimeLabel->setStyleSheet("font-size: 11px; color: #666666;");

    progressSlider = new QSlider(Qt::Horizontal);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(0);
    connect(progressSlider, &QSlider::valueChanged, this, &PlayerWidget::onProgressChanged);

    totalTimeLabel = new QLabel("0:00");
    totalTimeLabel->setStyleSheet("font-size: 11px; color: #666666;");

    progressLayout->addWidget(currentTimeLabel);
    progressLayout->addWidget(progressSlider, 1);
    progressLayout->addWidget(totalTimeLabel);

    controlsLayout->addLayout(buttonsLayout);
    controlsLayout->addLayout(progressLayout);

    // === Right Section: Volume ===
    volumeWidget = new QWidget(this);
    volumeWidget->setFixedWidth(150);
    volumeLayout = new QHBoxLayout(volumeWidget);
    volumeLayout->setContentsMargins(0, 0, 0, 0);
    volumeLayout->setSpacing(10);

    volumeButton = createControlButton("🔊", 24);

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(70);
    connect(volumeSlider, &QSlider::valueChanged, this, &PlayerWidget::onVolumeChanged);

    volumeLayout->addWidget(volumeButton);
    volumeLayout->addWidget(volumeSlider);

    // Add all sections to main layout
    mainLayout->addWidget(songInfoWidget);
    mainLayout->addWidget(controlsWidget, 1);
    mainLayout->addWidget(volumeWidget);
}

void PlayerWidget::applyStyles()
{
    setStyleSheet(
        "QWidget {"
        "   background-color: #FFFFFF;"
        "   border-top: 1px solid #e0e0e0;"
        "}"
        "QPushButton {"
        "   background-color: white;"
        "   color: #666666;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "   border: 1px solid #000000;"
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
        "   border: 1px solid #000000;"
        "   background-color: #f0f0f0;"
        "}"
        "QSlider::groove:horizontal {"
        "   background: #e0e0e0;"
        "   height: 4px;"
        "   border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: #000000;"
        "   width: 12px;"
        "   height: 12px;"
        "   margin: -4px 0;"
        "   border-radius: 6px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "   background: #333333;"
        "}"
        "QSlider::sub-page:horizontal {"
        "   background: #666666;"
        "   border-radius: 2px;"
        "}"
        "QLabel {"
        "   color: #666666;"
        "}"
    );

    playPauseButton->setObjectName("playPauseButton");
    playPauseButton->setFixedSize(40, 40);
}

QPushButton* PlayerWidget::createControlButton(const QString &icon, int size)
{
    QPushButton *button = new QPushButton(icon);
    button->setFixedSize(size, size);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void PlayerWidget::onPlayPauseClicked()
{
    isPlaying = !isPlaying;
    playPauseButton->setText(isPlaying ? "⏸" : "▶");
    // TODO: Implement actual playback control
}

void PlayerWidget::onPreviousClicked()
{
    // TODO: Implement previous track
}

void PlayerWidget::onNextClicked()
{
    // TODO: Implement next track
}

void PlayerWidget::onShuffleClicked()
{
    isShuffle = !isShuffle;
    shuffleButton->setProperty("active", isShuffle);
    shuffleButton->style()->unpolish(shuffleButton);
    shuffleButton->style()->polish(shuffleButton);
}

void PlayerWidget::onRepeatClicked()
{
    isRepeat = !isRepeat;
    repeatButton->setProperty("active", isRepeat);
    repeatButton->style()->unpolish(repeatButton);
    repeatButton->style()->polish(repeatButton);
}

void PlayerWidget::onProgressChanged(int value)
{
    Q_UNUSED(value);
    // TODO: Implement seek functionality
}

void PlayerWidget::onVolumeChanged(int value)
{
    // Update volume icon based on level
    if (value == 0) {
        volumeButton->setText("🔇");
    } else if (value < 50) {
        volumeButton->setText("🔉");
    } else {
        volumeButton->setText("🔊");
    }
    // TODO: Implement actual volume control
}
