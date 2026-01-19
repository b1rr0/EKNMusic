#include "downloadedpage.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QTimer>
#include <QMovie>

DownloadedSongsPage::DownloadedSongsPage(QWidget *parent)
    : QWidget(parent)
    , musicStorage(MusicStorageService::instance())
    , playerService(PlayerService::instance())
{
    setupUI();
    loadDownloadedSongs();

    // Connect to music storage changes
    connect(musicStorage, &MusicStorageService::tracksChanged,
            this, &DownloadedSongsPage::refreshSongList);
}

DownloadedSongsPage::~DownloadedSongsPage()
{
}

void DownloadedSongsPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Light background
    setStyleSheet(
        "QWidget {"
        "   background-color: #f5f5f5;"
        "}"
    );

    // Header layout with title and refresh button
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Simple title
    titleLabel = new QLabel("Downloaded Songs", this);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: 600;"
        "color: #000000;"
        "margin-bottom: 5px;"
    );

    // Refresh button
    QPushButton *refreshBtn = new QPushButton("⟳");
    refreshBtn->setFixedSize(40, 40);
    refreshBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f0f0f0;"
        "   color: #333333;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 20px;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4a9eff;"
        "   color: white;"
        "   border: 1px solid #4a9eff;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3080dd;"
        "}"
    );
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setToolTip("Refresh song list from folder");
    connect(refreshBtn, &QPushButton::clicked, this, &DownloadedSongsPage::onRefreshButtonClicked);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(refreshBtn);

    // Simple info label
    infoLabel = new QLabel("0 songs • 0 MB", this);
    infoLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #666666;"
        "margin-bottom: 25px;"
    );

    // Simple song list widget with drag & drop
    songListWidget = new QListWidget(this);
    songListWidget->setSpacing(0); // NO spacing between items (Spotify style)
    songListWidget->setDragEnabled(true);
    songListWidget->setAcceptDrops(true);
    songListWidget->setDropIndicatorShown(true);
    songListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    songListWidget->setDefaultDropAction(Qt::MoveAction);
    songListWidget->setStyleSheet(
        "QListWidget {"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QListWidget::item {"
        "   background-color: white;"
        "   border: none;"
        "   border-bottom: 1px solid #e0e0e0;"
        "   border-radius: 0px;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #fafafa;"
        "   cursor: pointer;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e8f4fd;"
        "   border-left: 3px solid #4a9eff;"
        "}"
        "QScrollBar:vertical {"
        "   background: transparent;"
        "   width: 8px;"
        "   margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #bdc3c7;"
        "   border-radius: 4px;"
        "   min-height: 30px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: #95a5a6;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   height: 0px;"
        "}"
    );

    connect(songListWidget, &QListWidget::itemClicked, this, &DownloadedSongsPage::onSongItemClicked);

    // Connect to model changed signal to detect reordering
    connect(songListWidget->model(), &QAbstractItemModel::rowsMoved,
            this, &DownloadedSongsPage::onSongOrderChanged);

    // Add to layout
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(songListWidget);
}

void DownloadedSongsPage::loadDownloadedSongs()
{
    songListWidget->clear();
    downloadedTracks = musicStorage->getDownloadedTracks();

    if (downloadedTracks.isEmpty()) {
        infoLabel->setText("No downloaded songs • Drop music files in: " + musicStorage->musicDirectory());
        return;
    }

    qint64 totalSize = 0;
    for (int i = 0; i < downloadedTracks.size(); ++i) {
        const Track &track = downloadedTracks[i];

        QListWidgetItem *item = new QListWidgetItem(songListWidget);
        QWidget *songWidget = createDownloadedSongItem(track, i);
        item->setSizeHint(songWidget->sizeHint());
        songListWidget->setItemWidget(item, songWidget);

        // IMPORTANT: Store the file path in item data so we can retrieve it after drag & drop
        item->setData(Qt::UserRole, track.filePath());

        // Calculate total size
        QFileInfo fileInfo(track.filePath());
        totalSize += fileInfo.size();
    }

    double totalSizeMB = totalSize / (1024.0 * 1024.0);
    infoLabel->setText(QString("%1 songs • %2 MB")
                           .arg(downloadedTracks.size())
                           .arg(totalSizeMB, 0, 'f', 1));
}

void DownloadedSongsPage::refreshSongList()
{
    loadDownloadedSongs();
}

QWidget* DownloadedSongsPage::createDownloadedSongItem(const Track &track, int index)
{
    QWidget *widget = new QWidget();
    widget->setMinimumHeight(64);
    widget->setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "}"
    );
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(16, 8, 16, 8);
    layout->setSpacing(12);

    // Play button/animation column (instead of index)
    bool isPlaying = (m_currentPlayingFile == track.filePath());

    QWidget *playWidget = new QWidget();playWidget->setObjectName("playWidget_" + QString::number(index));
	playWidget->setFixedSize(32, 32);  // Увеличил контейнер для гифки
	playWidget->setCursor(Qt::PointingHandCursor);

	QLabel *playIconLabel = new QLabel(playWidget);
	playIconLabel->setScaledContents(true);
	playIconLabel->setAlignment(Qt::AlignCenter);

	if (isPlaying) {
    	// Show animated GIF for currently playing track (bigger)
    	playIconLabel->setGeometry(0, 0, 32, 32);  // Гифка 32x32
    	QMovie *movie = new QMovie(":/images/src/resources/images/songAnimation.gif", QByteArray(), playWidget);
    	playIconLabel->setMovie(movie);
    	movie->start();
	} else {
    	// Show static play button icon (original size)
    	playIconLabel->setGeometry(8, 8, 16, 16);  // Иконка остается 16x16
    	QPixmap playIcon(":/images/src/resources/images/playButton.png");
    	playIconLabel->setPixmap(playIcon.scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

    // Make widget clickable
    QPushButton *invisibleBtn = new QPushButton(playWidget);
    invisibleBtn->setGeometry(0, 0, 32, 32);
    invisibleBtn->setStyleSheet("QPushButton { background: transparent; border: none; }");
    invisibleBtn->setCursor(Qt::PointingHandCursor);
    connect(invisibleBtn, &QPushButton::clicked, this, [this, index, track]() {
        m_currentPlayingFile = track.filePath();
        onPlayButtonClicked(index);
        refreshSongList(); // Refresh to show animation
    });

    // Album art column (show actual album art from track)
    QLabel *albumArtLabel = new QLabel();
    albumArtLabel->setFixedSize(48, 48);
    albumArtLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 4px;"
    );

    // Load album art if available
    if (!track.albumArt().isNull()) {
        QPixmap scaledArt = track.albumArt().scaled(48, 48, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        albumArtLabel->setPixmap(scaledArt);
        albumArtLabel->setScaledContents(true);
    } else {
        // Fallback music icon
        QLabel *fallbackIcon = new QLabel("♪", albumArtLabel);
        fallbackIcon->setAlignment(Qt::AlignCenter);
        fallbackIcon->setGeometry(0, 0, 48, 48);
        fallbackIcon->setStyleSheet("color: #888888; font-size: 20px; background: transparent;");
    }

    // Title & Artist column (stacked vertically)
    QWidget *titleArtistWidget = new QWidget();
    titleArtistWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *titleArtistLayout = new QVBoxLayout(titleArtistWidget);
    titleArtistLayout->setSpacing(2);
    titleArtistLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *titleLabel = new QLabel(track.title());
    titleLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #000000;"
        "background: transparent;"
    );
    titleLabel->setWordWrap(false);

    QLabel *artistLabel = new QLabel(track.artist());
    artistLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #888888;"
        "background: transparent;"
    );
    artistLabel->setWordWrap(false);

    titleArtistLayout->addWidget(titleLabel);
    titleArtistLayout->addWidget(artistLabel);

    // Date added column
    QString dateText = formatDateAdded(track.dateAdded());
    QLabel *dateLabel = new QLabel(dateText);
    dateLabel->setFixedWidth(120);
    dateLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "background: transparent;"
    );
    dateLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Duration column
    QString durationText = formatDuration(track.duration());
    QLabel *durationLabel = new QLabel(durationText);
    durationLabel->setFixedWidth(80);
    durationLabel->setAlignment(Qt::AlignCenter);
    durationLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "background: transparent;"
    );

    // File size column
    QString sizeText = formatFileSize(track.fileSize());
    QLabel *sizeLabel = new QLabel(sizeText);
    sizeLabel->setFixedWidth(100);
    sizeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    sizeLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "background: transparent;"
    );

    // Delete button column with xButton.png (no circle border)
    QPushButton *deleteBtn = new QPushButton();
    deleteBtn->setFixedSize(40, 40);

    // Set icon from resources (smaller to fit fully)
    QPixmap xButtonIcon(":/images/src/resources/images/xButton.png");
    QIcon xIcon(xButtonIcon);
    deleteBtn->setIcon(xIcon);
    deleteBtn->setIconSize(QSize(16, 16));

    deleteBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ffdddd;"
        "}"
    );
    deleteBtn->setCursor(Qt::PointingHandCursor);
    deleteBtn->setToolTip("Delete song");
    connect(deleteBtn, &QPushButton::clicked, this, [this, track]() {
        onDeleteButtonClicked(track.filePath());
    });

    // Build layout
    layout->addWidget(playWidget);
    layout->addWidget(albumArtLabel);
    layout->addWidget(titleArtistWidget, 1); // Stretch factor 1
    layout->addWidget(dateLabel);
    layout->addWidget(durationLabel);
    layout->addWidget(sizeLabel);
    layout->addWidget(deleteBtn);

    return widget;
}

void DownloadedSongsPage::onSongItemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    // Double-click handling can be implemented here
}

void DownloadedSongsPage::onPlayButtonClicked(int index)
{
    if (index >= 0 && index < downloadedTracks.size()) {
        // Set playlist and play selected track
        playerService->setPlaylist(downloadedTracks);
        // The setPlaylist method already starts playing the first track,
        // but we want to play the selected one, so we need to adjust
        for (int i = 0; i < index; ++i) {
            playerService->next();
        }
    }
}

void DownloadedSongsPage::onDeleteButtonClicked(const QString &filePath)
{
    if (!filePath.isEmpty()) {
        // Delete the actual file from filesystem
        musicStorage->deleteTrack(filePath);
        // List will be refreshed automatically via tracksChanged signal
    }
}

void DownloadedSongsPage::onSongOrderChanged()
{
    // The song order has been changed by drag and drop
    qDebug() << "Song order changed - saving new order";

    // Collect file paths in the NEW visual order (after drag & drop)
    QList<QString> orderedFilePaths;

    // Iterate through items in their current visual order
    for (int i = 0; i < songListWidget->count(); ++i) {
        QListWidgetItem *item = songListWidget->item(i);
        if (item) {
            // Retrieve the file path we stored in Qt::UserRole
            QString filePath = item->data(Qt::UserRole).toString();
            if (!filePath.isEmpty()) {
                orderedFilePaths.append(filePath);
                qDebug() << "  Position" << i << ":" << filePath;
            }
        }
    }

    // Update the order in storage (saves to JSON immediately)
    musicStorage->updateTrackOrder(orderedFilePaths);

    qDebug() << "New order saved to playlist.json with" << orderedFilePaths.size() << "tracks";
}

void DownloadedSongsPage::onRefreshButtonClicked()
{
    qDebug() << "Refreshing song list from folder...";

    // Force refresh the list from the actual folder
    // This will rescan the folder and pick up any new files
    // or restore files that were previously deleted
    refreshSongList();

    // Show feedback to user
    infoLabel->setText(infoLabel->text() + " • Refreshed!");

    // Reset the text after 2 seconds
    QTimer::singleShot(2000, this, [this]() {
        // Recalculate the info
        qint64 totalSize = 0;
        for (const Track &track : downloadedTracks) {
            QFileInfo fileInfo(track.filePath());
            totalSize += fileInfo.size();
        }
        double totalSizeMB = totalSize / (1024.0 * 1024.0);
        infoLabel->setText(QString("%1 songs • %2 MB")
                              .arg(downloadedTracks.size())
                              .arg(totalSizeMB, 0, 'f', 1));
    });
}

QString DownloadedSongsPage::formatDateAdded(const QDateTime &dateTime) const
{
    if (!dateTime.isValid()) {
        return "Unknown";
    }

    QDate today = QDate::currentDate();
    QDate addedDate = dateTime.date();

    if (addedDate == today) {
        return "Today";
    } else if (addedDate == today.addDays(-1)) {
        return "Yesterday";
    } else {
        return addedDate.toString("MMM d, yyyy");
    }
}

QString DownloadedSongsPage::formatFileSize(qint64 bytes) const
{
    if (bytes <= 0) {
        return "0 MB";
    }

    double mb = bytes / (1024.0 * 1024.0);
    if (mb < 10.0) {
        return QString::number(mb, 'f', 1) + " MB";
    } else {
        return QString::number(mb, 'f', 0) + " MB";
    }
}

QString DownloadedSongsPage::formatDuration(qint64 milliseconds) const
{
    if (milliseconds <= 0) {
        return "--:--";
    }

    int seconds = milliseconds / 1000;
    int minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

// Animation now handled by QMovie with GIF, so this function is no longer needed
