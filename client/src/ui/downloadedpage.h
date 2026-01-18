#ifndef DOWNLOADEDPAGE_H
#define DOWNLOADEDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>

class DownloadedSongsPage : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadedSongsPage(QWidget *parent = nullptr);
    ~DownloadedSongsPage();

private slots:
    void onSongItemClicked(QListWidgetItem *item);

private:
    void setupUI();
    void addMockData();
    QWidget* createDownloadedSongItem(const QString &title, const QString &artist,
                                      const QString &size, const QString &duration);

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *infoLabel;
    QListWidget *songListWidget;
};

#endif // DOWNLOADEDPAGE_H
