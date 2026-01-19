#include "downloadedpage.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QTimer>

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

    // Simple background
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
    songListWidget->setSpacing(12); // Add spacing between items
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
        "   border-radius: 12px;"
        "   border: 1px solid #e0e0e0;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #fafafa;"
        "   border: 1px solid #d0d0d0;"
        "   cursor: move;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e8f4fd;"
        "   border: 2px solid #4a9eff;"
        "}"
        "QListWidget::item:selected:active {"
        "   background-color: #d0e9ff;"
        "   border: 2px dashed #4a9eff;"
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
    widget->setMinimumHeight(80);
    widget->setStyleSheet(
        "QWidget {"
        "   background-color: transparent;"
        "   border-radius: 12px;"
        "}"
    );
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(20, 15, 20, 15);
    layout->setSpacing(15);

    // Drag handle icon
    QLabel *dragHandle = new QLabel("⋮⋮");
    dragHandle->setFixedSize(20, 36);
    dragHandle->setAlignment(Qt::AlignCenter);
    dragHandle->setStyleSheet(
        "color: #cccccc;"
        "font-size: 14px;"
        "font-weight: bold;"
        "background: transparent;"
    );
    dragHandle->setCursor(Qt::OpenHandCursor);
    dragHandle->setToolTip("Drag to reorder");

    // Music icon
    QLabel *iconLabel = new QLabel("♪");
    iconLabel->setFixedSize(36, 36);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 18px;"
        "color: #000000;"
        "font-size: 18px;"
        "font-weight: bold;"
    );

    // Song info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel(track.title());
    titleLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #000000;"
    );
    titleLabel->setWordWrap(false);

    QLabel *artistLabel = new QLabel(track.artist());
    artistLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #888888;"
    );
    artistLabel->setWordWrap(false);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(artistLabel);

    // File size
    QFileInfo fileInfo(track.filePath());
    double sizeMB = fileInfo.size() / (1024.0 * 1024.0);
    QLabel *sizeLabel = new QLabel(QString("%1 MB").arg(sizeMB, 0, 'f', 1));
    sizeLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "padding: 5px;"
    );
    sizeLabel->setMinimumWidth(70);

    // Duration (placeholder for now)
    QLabel *durationLabel = new QLabel("--:--");
    durationLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "padding: 5px;"
    );
    durationLabel->setMinimumWidth(50);

    // Delete button with trash icon
    QPushButton *deleteBtn = new QPushButton("✕");
    deleteBtn->setFixedSize(36, 36);
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #999999;"
        "   border: 1px solid #e0e0e0;"
        "   border-radius: 18px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff4444;"
        "   color: white;"
        "   border: 1px solid #ff4444;"
        "}"
    );
    deleteBtn->setCursor(Qt::PointingHandCursor);
    deleteBtn->setToolTip("Delete song");
    connect(deleteBtn, &QPushButton::clicked, this, [this, track]() {
        onDeleteButtonClicked(track.filePath());
    });

    // Simple play button
    QPushButton *playBtn = new QPushButton("▶");
    playBtn->setFixedSize(42, 42);
    playBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #000000;"
        "   color: #FFFFFF;"
        "   border: none;"
        "   border-radius: 21px;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #333333;"
        "}"
    );
    playBtn->setCursor(Qt::PointingHandCursor);
    connect(playBtn, &QPushButton::clicked, this, [this, index]() {
        onPlayButtonClicked(index);
    });

    layout->addWidget(dragHandle);
    layout->addWidget(iconLabel);
    layout->addLayout(infoLayout, 1);
    layout->addWidget(sizeLabel);
    layout->addWidget(durationLabel);
    layout->addWidget(deleteBtn);
    layout->addWidget(playBtn);

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
