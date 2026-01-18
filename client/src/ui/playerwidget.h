#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QStyle>

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget();

private slots:
    void onPlayPauseClicked();
    void onPreviousClicked();
    void onNextClicked();
    void onShuffleClicked();
    void onRepeatClicked();
    void onProgressChanged(int value);
    void onVolumeChanged(int value);

private:
    void setupUI();
    void applyStyles();
    QPushButton* createControlButton(const QString &icon, int size = 32);

    // Layout
    QHBoxLayout *mainLayout;

    // Left section (Song info)
    QWidget *songInfoWidget;
    QHBoxLayout *songInfoLayout;
    QLabel *albumArtLabel;
    QLabel *songTitleLabel;
    QLabel *songArtistLabel;
    QPushButton *likeButton;

    // Center section (Player controls)
    QWidget *controlsWidget;
    QVBoxLayout *controlsLayout;
    QHBoxLayout *buttonsLayout;
    QPushButton *shuffleButton;
    QPushButton *previousButton;
    QPushButton *playPauseButton;
    QPushButton *nextButton;
    QPushButton *repeatButton;
    QSlider *progressSlider;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;

    // Right section (Volume)
    QWidget *volumeWidget;
    QHBoxLayout *volumeLayout;
    QPushButton *volumeButton;
    QSlider *volumeSlider;

    // State
    bool isPlaying;
    bool isShuffle;
    bool isRepeat;
};

#endif // PLAYERWIDGET_H
