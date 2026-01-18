#include "searchpage.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>

SearchPage::SearchPage(QWidget *parent)
    : QWidget(parent)
{
    // Initialize mock data
    allSongs = {
        {"Blinding Lights", "The Weeknd", "After Hours"},
        {"Shape of You", "Ed Sheeran", "÷ (Divide)"},
        {"Someone Like You", "Adele", "21"},
        {"Uptown Funk", "Mark Ronson", "Uptown Special"},
        {"Bohemian Rhapsody", "Queen", "A Night at the Opera"},
        {"Hotel California", "Eagles", "Hotel California"},
        {"Imagine", "John Lennon", "Imagine"},
        {"Smells Like Teen Spirit", "Nirvana", "Nevermind"},
        {"Billie Jean", "Michael Jackson", "Thriller"},
        {"Yesterday", "The Beatles", "Help!"},
        {"Rolling in the Deep", "Adele", "21"},
        {"Thinking Out Loud", "Ed Sheeran", "x (Multiply)"},
        {"Closer", "The Chainsmokers", "Collage"},
        {"Despacito", "Luis Fonsi", "Vida"},
        {"Perfect", "Ed Sheeran", "÷ (Divide)"}
    };

    setupUI();
}

SearchPage::~SearchPage()
{
}

void SearchPage::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Simple title
    titleLabel = new QLabel("Search", this);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: 600;"
        "color: #000000;"
        "margin-bottom: 5px;"
    );

    // Simple search input
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("🔍 What do you want to listen to?");
    searchInput->setMinimumHeight(50);
    searchInput->setStyleSheet(
        "QLineEdit {"
        "   background-color: #FFFFFF;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 25px;"
        "   padding: 0 20px;"
        "   font-size: 14px;"
        "   color: #000000;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #000000;"
        "   background-color: #fafafa;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #999999;"
        "}"
    );
    connect(searchInput, &QLineEdit::textChanged, this, &SearchPage::onSearchTextChanged);

    // Simple results list
    resultsListWidget = new QListWidget(this);
    resultsListWidget->setStyleSheet(
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
    connect(resultsListWidget, &QListWidget::itemClicked, this, &SearchPage::onSearchItemClicked);

    // No results label
    noResultsLabel = new QLabel("Search for songs, artists, or albums", this);
    noResultsLabel->setAlignment(Qt::AlignCenter);
    noResultsLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #666666;"
        "margin-top: 50px;"
    );

    // Add to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(searchInput);
    mainLayout->addWidget(resultsListWidget);
    mainLayout->addWidget(noResultsLabel);
    mainLayout->addStretch();

    resultsListWidget->hide();
}

void SearchPage::onSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        resultsListWidget->hide();
        noResultsLabel->setText("Search for songs, artists, or albums");
        noResultsLabel->show();
    } else {
        performSearch(text);
    }
}

void SearchPage::performSearch(const QString &query)
{
    resultsListWidget->clear();
    QString lowerQuery = query.toLower();

    QList<Song> results;
    for (const auto &song : allSongs) {
        if (song.title.toLower().contains(lowerQuery) ||
            song.artist.toLower().contains(lowerQuery) ||
            song.album.toLower().contains(lowerQuery)) {
            results.append(song);
        }
    }

    if (results.isEmpty()) {
        resultsListWidget->hide();
        noResultsLabel->setText("No results found for \"" + query + "\"");
        noResultsLabel->show();
    } else {
        noResultsLabel->hide();
        resultsListWidget->show();

        for (const auto &song : results) {
            QListWidgetItem *item = new QListWidgetItem(resultsListWidget);
            QWidget *songWidget = createSearchResultItem(song.title, song.artist, song.album);
            item->setSizeHint(songWidget->sizeHint());
            resultsListWidget->setItemWidget(item, songWidget);
        }
    }
}

QWidget* SearchPage::createSearchResultItem(const QString &title, const QString &artist, const QString &album)
{
    QWidget *widget = new QWidget();
    widget->setMinimumHeight(70);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(15, 10, 15, 10);
    layout->setSpacing(15);

    // Simple album art placeholder
    QLabel *albumArtLabel = new QLabel();
    albumArtLabel->setFixedSize(55, 55);
    albumArtLabel->setStyleSheet(
        "background-color: #e0e0e0;"
        "border: 1px solid #d0d0d0;"
        "border-radius: 4px;"
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

    QLabel *artistLabel = new QLabel(artist + " • " + album);
    artistLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #888888;"
    );
    artistLabel->setWordWrap(false);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(artistLabel);

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

    // Simple add button
    QPushButton *addBtn = new QPushButton("+");
    addBtn->setFixedSize(36, 36);
    addBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: #000000;"
        "   border: 1px solid #d0d0d0;"
        "   border-radius: 18px;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0f0f0;"
        "   border: 1px solid #000000;"
        "}"
    );
    addBtn->setCursor(Qt::PointingHandCursor);

    layout->addWidget(albumArtLabel);
    layout->addLayout(infoLayout, 1);
    layout->addWidget(addBtn);
    layout->addWidget(playBtn);

    return widget;
}

void SearchPage::onSearchItemClicked(QListWidgetItem *item)
{
    // Handle search result click (play song in future iterations)
    Q_UNUSED(item);
}
