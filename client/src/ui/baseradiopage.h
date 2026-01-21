#ifndef BASERADIOPAGE_H
#define BASERADIOPAGE_H

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
#include <QScrollArea>
#include <QListWidget>
#include "services/radioservice.h"

/**
 * @brief Base class for all radio pages
 *
 * Provides common functionality for radio streaming UI:
 * - Background with album art
 * - Left panel with current player
 * - Right panel with song list (history + queue)
 * - Station title overlay
 *
 * Subclasses should implement specific station configurations
 */
class BaseRadioPage : public QWidget
{
    Q_OBJECT

public:
    explicit BaseRadioPage(RadioService *radioService, QWidget *parent = nullptr);
    virtual ~BaseRadioPage();

    virtual void loadRadioBackground();

protected:
    // Event handlers
    void resizeEvent(QResizeEvent *event) override;

    // Virtual methods for customization
    virtual QString getStationName() const = 0;
    virtual QString getStationId() const = 0;
    virtual QString getRequestSongUrl() const = 0;

    // UI setup methods
    virtual void setupUI();
    virtual void setupBackgroundLayer();
    virtual void setupLeftPanel(QHBoxLayout *mainLayout);
    virtual void setupRightPanel(QHBoxLayout *mainLayout);
    virtual void connectSignals();

    // Song list methods
    void updateSongList();
    void addSongToList(const RadioService::SongInfo &song, const QString &label, bool isCurrent = false);

protected slots:
    virtual void onNowPlayingUpdated(const RadioService::NowPlayingInfo &info);
    virtual void onPlaybackStateChanged(bool isPlaying);
    virtual void onPlayPauseClicked();
    virtual void onVolumeSliderChanged(int value);
    virtual void onVolumeBtnClicked();
    virtual void onRequestSongClicked();
    virtual void updateProgressBar();
    virtual void onSongHistoryReceived(const QList<RadioService::SongInfo> &history);
    virtual void onQueueReceived(const QList<RadioService::SongInfo> &queue);

private:
    void updateBackgroundImage(const QString &imageUrl);

protected:
    // Background layer
    QLabel *backgroundLabel;
    QWidget *overlayWidget;
    QLabel *stationTitle;

    // Left panel - Player
    QWidget *leftPanel;
    QLabel *albumArtLabel;
    QLabel *songTitleLabel;
    QLabel *artistLabel;
    QLabel *timeCurrentLabel;
    QLabel *timeTotalLabel;
    QSlider *progressBar;
    QPushButton *playPauseBtn;
    QPushButton *volumeBtn;
    QSlider *volumeSlider;

    // Bottom button
    QPushButton *requestSongBtn;

    // Right panel - Song list
    QWidget *rightPanel;
    QListWidget *songListWidget;

    // Services & Data
    RadioService *m_radioService;
    QNetworkAccessManager *networkManager;
    QTimer *updateTimer;
    QString currentBackgroundUrl;
    int currentDuration;
    int currentElapsed;

    // Song data cache
    QList<RadioService::SongInfo> m_songHistory;
    QList<RadioService::SongInfo> m_queue;

    // Song list order counter
    int m_songOrderCounter;

    // Previous state for change detection
    QString m_previousCurrentSongTitle;
    QList<RadioService::SongInfo> m_previousHistory;
    QList<RadioService::SongInfo> m_previousQueue;
};

#endif // BASERADIOPAGE_H
