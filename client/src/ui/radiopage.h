#ifndef RADIOPAGE_H
#define RADIOPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>

class RadioPage : public QWidget
{
    Q_OBJECT

public:
    explicit RadioPage(QWidget *parent = nullptr);
    ~RadioPage();

    void loadRadioBackground();

private slots:
    void onBackgroundDownloaded(QNetworkReply *reply);

private:
    void setupUI();
    void fetchBackgroundFromAPI();

    QVBoxLayout *mainLayout;
    QLabel *backgroundLabel;
    QLabel *titleLabel;

    QNetworkAccessManager *networkManager;
    QString currentBackgroundUrl;
};

#endif // RADIOPAGE_H
