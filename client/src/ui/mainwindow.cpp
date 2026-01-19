#include "mainwindow.h"
#include "searchpage.h"
#include "downloadedpage.h"
#include "playerwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    applyStyles();

    resize(1200, 800);
    setWindowTitle("EKNMusic");

    // Show Search page by default
    showSearch();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    createSidebar();
    createContent();
}

void MainWindow::createSidebar()
{
    sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(280);

    sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(15);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);

    // Logo with image (increased by 50%: 100 -> 150)
    logoLabel = new QLabel(sidebar);
    logoLabel->setObjectName("logo");
    logoLabel->setAlignment(Qt::AlignCenter);
    QPixmap logo(":/images/src/resources/images/logo.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        logoLabel->setText("♫ EKNMUSIC");
        logoLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #000000;");
    }
    logoLabel->setStyleSheet("margin-bottom: 20px;");

    // Navigation buttons (removed Liked Songs)
    searchBtn = createNavButton("🔍 Search", "🔍");
    downloadedBtn = createNavButton("⬇ Downloads", "⬇");

    // Connect signals
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::showSearch);
    connect(downloadedBtn, &QPushButton::clicked, this, &MainWindow::showDownloaded);

    // Add widgets to sidebar
    sidebarLayout->addWidget(logoLabel);
    sidebarLayout->addWidget(searchBtn);
    sidebarLayout->addWidget(downloadedBtn);
    sidebarLayout->addStretch();

    mainLayout->addWidget(sidebar);
}

void MainWindow::createContent()
{
    contentArea = new QWidget(this);
    contentArea->setObjectName("contentArea");

    contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Create stacked widget for pages
    stackedWidget = new QStackedWidget(contentArea);

    // Create pages (removed Liked Songs)
    searchPage = new SearchPage(stackedWidget);
    downloadedPage = new DownloadedSongsPage(stackedWidget);

    // Add pages to stacked widget
    stackedWidget->addWidget(searchPage);
    stackedWidget->addWidget(downloadedPage);

    // Create player widget
    playerWidget = new PlayerWidget(contentArea);
    playerWidget->setFixedHeight(90);

    // Add to content layout
    contentLayout->addWidget(stackedWidget);
    contentLayout->addWidget(playerWidget);

    mainLayout->addWidget(contentArea);
}

void MainWindow::applyStyles()
{
    setStyleSheet(
        "#sidebar {"
        "   background-color: #ffffff;"
        "   border-right: 1px solid #e0e0e0;"
        "}"
        "#contentArea {"
        "   background-color: #f5f5f5;"
        "}"
        "QPushButton {"
        "   background-color: transparent;"
        "   color: #666666;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 16px;"
        "   text-align: left;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f5f5f5;"
        "   color: #000000;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e8e8e8;"
        "}"
        "QPushButton[active=\"true\"] {"
        "   background-color: #000000;"
        "   color: #FFFFFF;"
        "}"
    );
}

QPushButton* MainWindow::createNavButton(const QString &text, const QString &iconText)
{
    QPushButton *button = new QPushButton(text, sidebar);
    button->setCursor(Qt::PointingHandCursor);
    button->setProperty("active", false);
    return button;
}

void MainWindow::showSearch()
{
    stackedWidget->setCurrentWidget(searchPage);
    searchBtn->setProperty("active", true);
    downloadedBtn->setProperty("active", false);

    // Refresh styles
    searchBtn->style()->unpolish(searchBtn);
    searchBtn->style()->polish(searchBtn);
    downloadedBtn->style()->unpolish(downloadedBtn);
    downloadedBtn->style()->polish(downloadedBtn);
}

void MainWindow::showDownloaded()
{
    stackedWidget->setCurrentWidget(downloadedPage);
    searchBtn->setProperty("active", false);
    downloadedBtn->setProperty("active", true);

    // Refresh styles
    searchBtn->style()->unpolish(searchBtn);
    searchBtn->style()->polish(searchBtn);
    downloadedBtn->style()->unpolish(downloadedBtn);
    downloadedBtn->style()->polish(downloadedBtn);
}
