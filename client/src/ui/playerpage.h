#ifndef PLAYERPAGE_H
#define PLAYERPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QMediaPlayer>
#include <QStyle>
#include "services/playerservice.h"
#include "models/track.h"

class PlayerPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerPage(QWidget *parent = nullptr);
    ~PlayerPage();

private slots:
    void onPlayPauseClicked();
    void onPreviousClicked();
    void onNextClicked();
    void onShuffleClicked();
    void onRepeatClicked();
    void onLikeClicked();
    void onProgressChanged(int value);
    void onVolumeChanged(int value);
    void onBackClicked();

    // PlayerService slots
    void onTrackChanged(const Track &track);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);

private:
    void setupUI();
    void applyStyles();
    void setupPlayerConnections();
    QPushButton* createControlButton(const QString &icon, int size);
    QString formatTime(qint64 milliseconds) const;

    // Layout
    QVBoxLayout *mainLayout;
    QPushButton *backButton;

    // Album art (large)
    QLabel *albumArtLabel;

    // Track info
    QLabel *songTitleLabel;
    QLabel *songArtistLabel;

    // Controls
    QWidget *controlsWidget;
    QHBoxLayout *controlButtonsLayout;
    QPushButton *shuffleButton;
    QPushButton *previousButton;
    QPushButton *playPauseButton;
    QPushButton *nextButton;
    QPushButton *repeatButton;

    // Progress bar
    QWidget *progressWidget;
    QHBoxLayout *progressLayout;
    QLabel *currentTimeLabel;
    QSlider *progressSlider;
    QLabel *totalTimeLabel;

    // Additional controls
    QWidget *additionalControlsWidget;
    QHBoxLayout *additionalControlsLayout;
    QPushButton *likeButton;
    QPushButton *volumeButton;
    QSlider *volumeSlider;

    // State
    bool isPlaying;
    bool isShuffle;
    bool isRepeat;
    bool isLiked;
    bool isSeekingByUser;

    // Player service reference
    PlayerService *playerService;
};

#endif // PLAYERPAGE_H
