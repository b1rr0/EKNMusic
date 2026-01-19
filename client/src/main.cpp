#include <QApplication>
#include "config/appconfig.h"
#include "ui/loginwindow.h"
#include "services/musicstorageservice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize application configuration
    AppConfig::initializeApp();

    // Initialize music storage service to ensure songs directory exists
    MusicStorageService::instance();

    // Create and show login window
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
