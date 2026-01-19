#include "playerwidget.h"
#include "playerpage.h"
#include <QPixmap>
#include <QStackedWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QFile>
#include <QDebug>

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
    , isPlaying(false)
    , isShuffle(false)
    , isRepeat(false)
    , isSeekingByUser(false)
    , playerService(PlayerService::instance())
{
    setupUI();
    applyStyles();
    setupPlayerConnections();
}

PlayerWidget::~PlayerWidget()
{
}

void PlayerWidget::setupUI()
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 10, 15, 10);
    mainLayout->setSpacing(0);

    // === Left Section: Song Info ===
    songInfoWidget = new QWidget(this);
    songInfoWidget->setFixedWidth(250);
    songInfoLayout = new QHBoxLayout(songInfoWidget);
    songInfoLayout->setContentsMargins(0, 0, 0, 0);
    songInfoLayout->setSpacing(10);

    // Album art (clickable)
    albumArtLabel = new QLabel();
    albumArtLabel->setFixedSize(60, 60);
    albumArtLabel->setScaledContents(true);
    albumArtLabel->setCursor(Qt::PointingHandCursor);
    albumArtLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 4px;"
    );
    albumArtLabel->installEventFilter(this);

    // Song title and artist (clickable)
    QVBoxLayout *songTextLayout = new QVBoxLayout();
    songTitleLabel = new QLabel("No song playing");
    songTitleLabel->setStyleSheet(
        "font-size: 13px; font-weight: 600; color: #000000;"
        "QLabel:hover { text-decoration: underline; }"
    );
    songTitleLabel->setCursor(Qt::PointingHandCursor);
    songTitleLabel->installEventFilter(this);

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
    controlsLayout->setSpacing(5);

    // Control buttons
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(10);
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
    progressLayout->setSpacing(8);

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
    volumeLayout->setSpacing(8);

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

void PlayerWidget::setupPlayerConnections()
{
    // Connect to PlayerService signals
    connect(playerService, &PlayerService::trackChanged,
            this, &PlayerWidget::onTrackChanged);
    connect(playerService, &PlayerService::playbackStateChanged,
            this, &PlayerWidget::onPlaybackStateChanged);
    connect(playerService, &PlayerService::positionChanged,
            this, &PlayerWidget::onPositionChanged);
    connect(playerService, &PlayerService::durationChanged,
            this, &PlayerWidget::onDurationChanged);

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
}

QString PlayerWidget::formatTime(qint64 milliseconds) const
{
    qint64 seconds = milliseconds / 1000;
    qint64 minutes = seconds / 60;
    seconds = seconds % 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

void PlayerWidget::onPlayPauseClicked()
{
    playerService->togglePlayPause();
}

void PlayerWidget::onPreviousClicked()
{
    playerService->previous();
}

void PlayerWidget::onNextClicked()
{
    playerService->next();
}

void PlayerWidget::onShuffleClicked()
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

void PlayerWidget::onRepeatClicked()
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

void PlayerWidget::onProgressChanged(int value)
{
    if (!isSeekingByUser) {
        currentTimeLabel->setText(formatTime(value));
    }
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

    // Update player service volume
    playerService->setVolume(value);
}

void PlayerWidget::onTrackChanged(const Track &track)
{
    qDebug() << "Track changed - Title:" << track.title() << "Artist:" << track.artist() << "Album art:" << track.albumArtPath();

    songTitleLabel->setText(track.title());
    songArtistLabel->setText(track.artist());

    // Load album art if available
    QString artPath = track.albumArtPath();
    if (!artPath.isEmpty() && QFile::exists(artPath)) {
        qDebug() << "Loading album art from:" << artPath;
        QPixmap albumArt(artPath);
        if (!albumArt.isNull()) {
            albumArtLabel->setPixmap(albumArt.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            qDebug() << "Album art loaded successfully";
        } else {
            qDebug() << "Failed to load album art - pixmap is null";
        }
    } else {
        qDebug() << "No album art available or file doesn't exist";
        // Set default placeholder
        albumArtLabel->clear();
        albumArtLabel->setStyleSheet(
            "background-color: #e0e0e0;"
            "border: 1px solid #d0d0d0;"
            "border-radius: 4px;"
        );
    }
}

void PlayerWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    isPlaying = (state == QMediaPlayer::PlayingState);
    playPauseButton->setText(isPlaying ? "⏸" : "▶");
}

void PlayerWidget::onPositionChanged(qint64 position)
{
    if (!isSeekingByUser) {
        progressSlider->setValue(position);
        currentTimeLabel->setText(formatTime(position));
    }
}

void PlayerWidget::onDurationChanged(qint64 duration)
{
    progressSlider->setRange(0, duration);
    totalTimeLabel->setText(formatTime(duration));
}

bool PlayerWidget::eventFilter(QObject *obj, QEvent *event)
{
    if ((obj == albumArtLabel || obj == songTitleLabel) && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            onAlbumArtClicked();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void PlayerWidget::onAlbumArtClicked()
{
    qDebug() << "Album art clicked!";

    // Find the stacked widget - it's a sibling widget in the parent's layout
    QWidget *parent = parentWidget();
    QStackedWidget *stackedWidget = nullptr;

    if (parent) {
        qDebug() << "Parent found:" << parent->objectName();
        // Look for QStackedWidget among siblings
        QList<QStackedWidget*> stackedWidgets = parent->findChildren<QStackedWidget*>();
        qDebug() << "Found" << stackedWidgets.count() << "stacked widgets";
        if (!stackedWidgets.isEmpty()) {
            stackedWidget = stackedWidgets.first();
        }
    }

    if (stackedWidget) {
        qDebug() << "StackedWidget found! Current index:" << stackedWidget->currentIndex();

        // Find or create PlayerPage
        PlayerPage *playerPage = nullptr;
        for (int i = 0; i < stackedWidget->count(); ++i) {
            playerPage = qobject_cast<PlayerPage*>(stackedWidget->widget(i));
            if (playerPage) {
                qDebug() << "Found existing PlayerPage at index" << i;
                break;
            }
        }

        if (!playerPage) {
            qDebug() << "Creating new PlayerPage";
            // Create new PlayerPage if it doesn't exist
            playerPage = new PlayerPage(stackedWidget);
            stackedWidget->addWidget(playerPage);

            // Connect signal to show/hide this widget when switching pages
            connect(stackedWidget, &QStackedWidget::currentChanged, this, [this, stackedWidget](int index) {
                // Hide PlayerWidget when on PlayerPage, show otherwise
                PlayerPage *pp = qobject_cast<PlayerPage*>(stackedWidget->widget(index));
                if (pp) {
                    this->hide();
                } else {
                    this->show();
                }
            });
        }

        // Switch to PlayerPage
        qDebug() << "Switching to PlayerPage";
        stackedWidget->setCurrentWidget(playerPage);
        qDebug() << "New index:" << stackedWidget->currentIndex();

        // Hide this player widget
        this->hide();
    } else {
        qDebug() << "ERROR: StackedWidget not found!";
    }
}
