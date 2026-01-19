#include "appconfig.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>

namespace AppConfig
{
    void initializeApp()
    {
        // Set application metadata
        QApplication::setApplicationName(APP_NAME);
        QApplication::setApplicationVersion(APP_VERSION);
        QApplication::setOrganizationName(ORGANIZATION_NAME);

        int fontId = QFontDatabase::addApplicationFont(FONT_PATH);
        if (fontId != -1) {
            QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
            if (!fontFamilies.isEmpty()) {
                QString fontFamily = fontFamilies.at(0);
                QFont animeFont(fontFamily, FONT_SIZE);
                QApplication::setFont(animeFont);
                qDebug() << "Anime Ace font loaded successfully:" << fontFamily;
            }
        } else {
            qWarning() << "Failed to load Anime Ace font from resources";
        }
    }
}
