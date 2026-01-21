#include "appconfig.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QDebug>
#include <QByteArray>

AppConfig* AppConfig::s_instance = nullptr;

AppConfig::AppConfig()
    : m_settings(new QSettings(ORGANIZATION_NAME, APP_NAME))
{
}

AppConfig::~AppConfig()
{
    delete m_settings;
}

AppConfig* AppConfig::instance()
{
    if (!s_instance) {
        s_instance = new AppConfig();
    }
    return s_instance;
}

void AppConfig::initializeApp()
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

    // Initialize instance for settings
    instance();

    // Set default API key if not already set (will be obfuscated)
    if (instance()->getRadioApiKey().isEmpty()) {
        // Default API key (user can change it later)
        instance()->setRadioApiKey("d2cc91b3ad579f96:e308c50246c6440bf68281050bfdd995");
    }
}

QString AppConfig::encodeKey(const QString &key)
{
    // Simple XOR obfuscation (not encryption, but better than plain text)
    QByteArray data = key.toUtf8();
    QByteArray obfuscated;
    const char xorKey = 0x7B; // Simple XOR key

    for (int i = 0; i < data.size(); ++i) {
        obfuscated.append(data[i] ^ xorKey);
    }

    return QString::fromLatin1(obfuscated.toBase64());
}

QString AppConfig::decodeKey(const QString &encoded)
{
    QByteArray obfuscated = QByteArray::fromBase64(encoded.toLatin1());
    QByteArray data;
    const char xorKey = 0x7B;

    for (int i = 0; i < obfuscated.size(); ++i) {
        data.append(obfuscated[i] ^ xorKey);
    }

    return QString::fromUtf8(data);
}

QString AppConfig::getRadioApiKey() const
{
    QString encoded = m_settings->value("radio/api_key_encoded", "").toString();
    if (encoded.isEmpty()) {
        return QString();
    }
    return decodeKey(encoded);
}

void AppConfig::setRadioApiKey(const QString &apiKey)
{
    QString encoded = encodeKey(apiKey);
    m_settings->setValue("radio/api_key_encoded", encoded);
    m_settings->sync();
    qDebug() << "Radio API key stored securely";
}
