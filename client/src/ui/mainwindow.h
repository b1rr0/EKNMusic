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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSearch();
    void showDownloaded();

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
    QPushButton *searchBtn;
    QPushButton *downloadedBtn;

    // Content area
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QStackedWidget *stackedWidget;

    // Pages
    SearchPage *searchPage;
    DownloadedSongsPage *downloadedPage;

    // Player
    PlayerWidget *playerWidget;
};

#endif // MAINWINDOW_H
