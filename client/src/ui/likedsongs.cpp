#include "likedsongs.h"
#include <QListWidgetItem>

LikedSongsPage::LikedSongsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    addMockData();
}

LikedSongsPage::~LikedSongsPage()
{
}

void LikedSongsPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Simple light background
    setStyleSheet(
        "QWidget {"
        "   background-color: #f5f5f5;"
        "}"
    );

    // Simple title - black and white
    titleLabel = new QLabel("Liked Songs", this);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: 600;"
        "color: #000000;"
        "margin-bottom: 5px;"
    );

    // Info label - gray
    infoLabel = new QLabel("0 songs", this);
    infoLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #666666;"
        "margin-bottom: 25px;"
    );

    // Simple list with cards
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

    connect(songListWidget, &QListWidget::itemClicked, this, &LikedSongsPage::onSongItemClicked);

    // Add to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(songListWidget);
}

void LikedSongsPage::addMockData()
{
    // Mock song data for demonstration
    struct Song {
        QString title;
        QString artist;
        QString duration;
    };

    QList<Song> mockSongs = {
        {"Blinding Lights", "The Weeknd", "3:20"},
        {"Shape of You", "Ed Sheeran", "3:53"},
        {"Someone Like You", "Adele", "4:45"},
        {"Uptown Funk", "Mark Ronson ft. Bruno Mars", "4:30"},
        {"Bohemian Rhapsody", "Queen", "5:55"},
        {"Hotel California", "Eagles", "6:30"},
        {"Imagine", "John Lennon", "3:03"},
        {"Smells Like Teen Spirit", "Nirvana", "5:01"},
        {"Billie Jean", "Michael Jackson", "4:54"},
        {"Yesterday", "The Beatles", "2:05"}
    };

    for (const auto &song : mockSongs) {
        QListWidgetItem *item = new QListWidgetItem(songListWidget);
        QWidget *songWidget = createSongItem(song.title, song.artist, song.duration);
        item->setSizeHint(songWidget->sizeHint());
        songListWidget->setItemWidget(item, songWidget);
    }

    infoLabel->setText(QString("%1 songs").arg(mockSongs.size()));
}

QWidget* LikedSongsPage::createSongItem(const QString &title, const QString &artist, const QString &duration)
{
    QWidget *widget = new QWidget();
    widget->setMinimumHeight(70); // Fix: установим минимальную высоту
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(15);

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
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QLabel *artistLabel = new QLabel(artist);
    artistLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #888888;"
    );
    artistLabel->setWordWrap(false);
    artistLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(artistLabel);

    // Duration - simple gray
    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #666666;"
        "padding: 5px;"
    );
    durationLabel->setAlignment(Qt::AlignCenter);
    durationLabel->setMinimumWidth(50);

    // Simple like button
    QPushButton *likeBtn = new QPushButton("♥");
    likeBtn->setFixedSize(36, 36);
    likeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #000000;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 18px;"
        "   font-size: 18px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "   border: 1px solid #000000;"
        "}"
    );
    likeBtn->setCursor(Qt::PointingHandCursor);

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

    layout->addLayout(infoLayout, 1);
    layout->addWidget(durationLabel);
    layout->addWidget(likeBtn);
    layout->addWidget(playBtn);

    return widget;
}

void LikedSongsPage::onSongItemClicked(QListWidgetItem *item)
{
    // Handle song item click (play song in future iterations)
    Q_UNUSED(item);
}
