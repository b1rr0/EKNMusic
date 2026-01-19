#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

namespace AppConfig
{
    // Application metadata
    constexpr const char* APP_NAME = "EKNMusic";
    constexpr const char* APP_VERSION = "1.0.0";
    constexpr const char* ORGANIZATION_NAME = "EKN";

    // Font settings
    constexpr const char* FONT_PATH = ":/fonts/src/resources/fonts/animeace2_reg.otf";
    constexpr int FONT_SIZE = 10;

    // Initialize application settings
    void initializeApp();
}

#endif // APPCONFIG_H
