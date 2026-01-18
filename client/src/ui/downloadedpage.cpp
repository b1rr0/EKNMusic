#include "downloadedpage.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>

DownloadedSongsPage::DownloadedSongsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    addMockData();
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

    // Simple title
    titleLabel = new QLabel("Downloaded Songs", this);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: 600;"
        "color: #000000;"
        "margin-bottom: 5px;"
    );

    // Simple info label
    infoLabel = new QLabel("0 songs • 0 MB", this);
    infoLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #666666;"
        "margin-bottom: 25px;"
    );

    // Simple song list widget
    songListWidget = new QListWidget(this);
    songListWidget->setStyleSheet(
        "QListWidget {"
        "   background-color: transparent;"
        "   border: none;"
        "   outline: none;"
        "}"
        "QListWidget::item {"
        "   background-color: white;"
        "   padding: 12px;"
        "   border-radius: 8px;"
        "   margin-bottom: 10px;"
        "   border: 1px solid #e0e0e0;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #fafafa;"
        "   box-shadow: 0 2px 8px rgba(0,0,0,0.1);"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #ecf0f1;"
        "   border: 1px solid #bdc3c7;"
        "}"
        "QScrollBar:vertical {"
        "   background: transparent;"
        "   width: 8px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #bdc3c7;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: #95a5a6;"
        "}"
    );

    connect(songListWidget, &QListWidget::itemClicked, this, &DownloadedSongsPage::onSongItemClicked);

    // Add to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(songListWidget);
}

void DownloadedSongsPage::addMockData()
{
    // Mock downloaded song data
    struct DownloadedSong {
        QString title;
        QString artist;
        QString size;
        QString duration;
    };

    QList<DownloadedSong> mockSongs = {
        {"Blinding Lights", "The Weeknd", "7.8 MB", "3:20"},
        {"Shape of You", "Ed Sheeran", "9.1 MB", "3:53"},
        {"Bohemian Rhapsody", "Queen", "13.6 MB", "5:55"},
        {"Hotel California", "Eagles", "14.9 MB", "6:30"},
        {"Imagine", "John Lennon", "7.1 MB", "3:03"}
    };

    double totalSize = 0;
    for (const auto &song : mockSongs) {
        QListWidgetItem *item = new QListWidgetItem(songListWidget);
        QWidget *songWidget = createDownloadedSongItem(song.title, song.artist, song.size, song.duration);
        item->setSizeHint(songWidget->sizeHint());
        songListWidget->setItemWidget(item, songWidget);

        // Calculate total size
        totalSize += song.size.split(" ")[0].toDouble();
    }

    infoLabel->setText(QString("%1 songs • %2 MB")
                           .arg(mockSongs.size())
                           .arg(totalSize, 0, 'f', 1));
}

QWidget* DownloadedSongsPage::createDownloadedSongItem(const QString &title, const QString &artist,
                                                       const QString &size, const QString &duration)
{
    QWidget *widget = new QWidget();
    widget->setMinimumHeight(70);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(15);

    // Simple download icon
    QLabel *iconLabel = new QLabel("✓");
    iconLabel->setFixedSize(36, 36);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 18px;"
        "color: #000000;"
        "font-size: 16px;"
        "font-weight: bold;"
    );

    // Song info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #000000;"
    );
    titleLabel->setWordWrap(false);

    QLabel *artistLabel = new QLabel(artist);
    artistLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #888888;"
    );
    artistLabel->setWordWrap(false);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(artistLabel);

    // File size
    QLabel *sizeLabel = new QLabel(size);
    sizeLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "padding: 5px;"
    );
    sizeLabel->setMinimumWidth(60);

    // Duration
    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "padding: 5px;"
    );
    durationLabel->setMinimumWidth(50);

    // Simple delete button
    QPushButton *deleteBtn = new QPushButton("🗑");
    deleteBtn->setFixedSize(36, 36);
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #666666;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 18px;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "   color: #333333;"
        "   border: 1px solid #000000;"
        "}"
    );
    deleteBtn->setCursor(Qt::PointingHandCursor);

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
    // Handle downloaded song click (play song in future iterations)
    Q_UNUSED(item);
}
