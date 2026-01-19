#include <QApplication>
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

    // Initialize music storage service to ensure songs directory exists
    MusicStorageService::instance();

    // Create and show login window
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
