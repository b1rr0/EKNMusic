#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>

class SearchPage : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPage(QWidget *parent = nullptr);
    ~SearchPage();

private slots:
    void onSearchTextChanged(const QString &text);
    void onSearchItemClicked(QListWidgetItem *item);

private:
    void setupUI();
    void performSearch(const QString &query);
    QWidget* createSearchResultItem(const QString &title, const QString &artist, const QString &album);

    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *searchInput;
    QListWidget *resultsListWidget;
    QLabel *noResultsLabel;

    // Mock data
    struct Song {
        QString title;
        QString artist;
        QString album;
    };
    QList<Song> allSongs;
};

#endif // SEARCHPAGE_H
