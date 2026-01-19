#ifndef DOWNLOADEDPAGE_H
#define DOWNLOADEDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include "models/track.h"
#include "services/musicstorageservice.h"
#include "services/playerservice.h"

class DownloadedSongsPage : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadedSongsPage(QWidget *parent = nullptr);
    ~DownloadedSongsPage();

    void refreshSongList();

private slots:
    void onSongItemClicked(QListWidgetItem *item);
    void onPlayButtonClicked(int index);
    void onDeleteButtonClicked(int index);

private:
    void setupUI();
    void loadDownloadedSongs();
    QWidget* createDownloadedSongItem(const Track &track, int index);

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *infoLabel;
    QListWidget *songListWidget;

    MusicStorageService *musicStorage;
    PlayerService *playerService;
    QList<Track> downloadedTracks;
};

#endif // DOWNLOADEDPAGE_H
