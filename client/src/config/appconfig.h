#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>
#include <QSettings>

class AppConfig
{
public:
    static AppConfig* instance();

    // Application metadata
    static constexpr const char* APP_NAME = "EKNMusic";
    static constexpr const char* APP_VERSION = "1.0.0";
    static constexpr const char* ORGANIZATION_NAME = "EKN";

    // Font settings
    static constexpr const char* FONT_PATH = ":/fonts/src/resources/fonts/animeace2_reg.otf";
    static constexpr int FONT_SIZE = 10;

    // Initialize application settings
    static void initializeApp();

    // Radio API Key management (stored securely)
    QString getRadioApiKey() const;
    void setRadioApiKey(const QString &apiKey);

private:
    AppConfig();
    ~AppConfig();
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    static AppConfig* s_instance;
    QSettings *m_settings;

    // Obfuscated storage key (basic security)
    static QString encodeKey(const QString &key);
    static QString decodeKey(const QString &encoded);
};

#endif // APPCONFIG_H
