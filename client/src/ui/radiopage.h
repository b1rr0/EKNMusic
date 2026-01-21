#ifndef RADIOPAGE_H
#define RADIOPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QTimer>
#include "services/radioservice.h"

class RadioPage : public QWidget
{
    Q_OBJECT

public:
    explicit RadioPage(QWidget *parent = nullptr);
    ~RadioPage();

    void loadRadioBackground();

private slots:
    void onNowPlayingUpdated(const RadioService::NowPlayingInfo &info);
    void onPlaybackStateChanged(bool isPlaying);
    void onPlayPauseClicked();
    void onVolumeSliderChanged(int value);
    void onVolumeBtnClicked();
    void onSongHistoryClicked();
    void onRequestSongClicked();
    void onPlaylistClicked();
    void updateProgressBar();

private:
    void setupUI();
    void updateBackgroundImage(const QString &imageUrl);
    void connectSignals();

    // Main layout
    QLabel *backgroundLabel;
    QWidget *overlayWidget;

    // Player card
    QWidget *playerCard;
    QLabel *stationTitle;
    QLabel *albumArtLabel;
    QLabel *songTitleLabel;
    QLabel *artistLabel;
    QLabel *timeCurrentLabel;
    QLabel *timeTotalLabel;
    QSlider *progressBar;
    QPushButton *playPauseBtn;
    QPushButton *volumeBtn;
    QSlider *volumeSlider;

    // Bottom buttons
    QPushButton *songHistoryBtn;
    QPushButton *requestSongBtn;
    QPushButton *playlistBtn;

    // Services & Data
    RadioService *m_radioService;
    QNetworkAccessManager *networkManager;
    QTimer *updateTimer;
    QString currentBackgroundUrl;
    int currentDuration;
    int currentElapsed;
};

#endif // RADIOPAGE_H
