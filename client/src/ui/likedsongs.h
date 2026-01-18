#ifndef LIKEDSONGS_H
#define LIKEDSONGS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>

class LikedSongsPage : public QWidget
{
    Q_OBJECT

public:
    explicit LikedSongsPage(QWidget *parent = nullptr);
    ~LikedSongsPage();

private slots:
    void onSongItemClicked(QListWidgetItem *item);

private:
    void setupUI();
    void addMockData();
    QWidget* createSongItem(const QString &title, const QString &artist, const QString &duration);

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *infoLabel;
    QListWidget *songListWidget;
};

#endif // LIKEDSONGS_H
