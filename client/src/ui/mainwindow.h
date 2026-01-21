#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QStyle>

// Forward declarations
class PlayerWidget;
class SearchPage;
class DownloadedSongsPage;
class EknmIntercomRadioPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSearch();
    void showDownloaded();
    void showRadio();
    void switchToSongsMode();
    void switchToRadioMode();

private:
    void setupUI();
    void createSidebar();
    void createContent();
    void applyStyles();
    QPushButton* createNavButton(const QString &text, const QString &iconText);

    // UI Components
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;

    // Sidebar
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;

    // Mode toggle
    QWidget *modeToggleWidget;
    QPushButton *songsBtn;
    QPushButton *radioBtn;

    // Navigation buttons (will change based on mode)
    QPushButton *searchBtn;
    QPushButton *downloadedBtn;
    QPushButton *eknmIntercomBtn;

    // Content area
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QStackedWidget *stackedWidget;

    // Pages
    SearchPage *searchPage;
    DownloadedSongsPage *downloadedPage;
    EknmIntercomRadioPage *radioPage;

    // Player
    PlayerWidget *playerWidget;

    // Current mode
    enum Mode { SONGS, RADIO };
    Mode currentMode;
};

#endif // MAINWINDOW_H
