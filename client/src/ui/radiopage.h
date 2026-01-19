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

class RadioPage : public QWidget
{
    Q_OBJECT

public:
    explicit RadioPage(QWidget *parent = nullptr);
    ~RadioPage();

    void loadRadioBackground();

private slots:
    void onNowPlayingDataReceived(QNetworkReply *reply);
    void updateNowPlaying();

private:
    void setupUI();
    void fetchNowPlayingData();
    void updateBackgroundImage(const QString &imageUrl);

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

    // Network & Data
    QNetworkAccessManager *networkManager;
    QTimer *updateTimer;
    QString currentBackgroundUrl;
    QString currentSongTitle;
    QString currentArtist;
    int currentDuration;
    int currentElapsed;
};

#endif // RADIOPAGE_H
