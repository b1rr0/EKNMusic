#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include "ui/mainwindow.h"
#include "ui/loginwindow.h"
#include "services/musicstorageservice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("EKNMusic");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("EKN");

    // Load Anime Ace font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/resources/fonts/animeace2_reg.otf");
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()) {
            QString fontFamily = fontFamilies.at(0);
            QFont animeFont(fontFamily, 10); // 10pt default size
            QApplication::setFont(animeFont);
            qDebug() << "Anime Ace font loaded successfully:" << fontFamily;
        }
    } else {
        qWarning() << "Failed to load Anime Ace font from resources";
    }

    // Initialize music storage service to ensure songs directory exists
    MusicStorageService::instance();

    // Create and show login window
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
