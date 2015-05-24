/* ==================================================
   Qt GUI project
   Created: Geoff R. McLane - Mar 2012
   License: GNU GPL3 (or later)
   ================================================== */
#include "app_config.h"
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationDomain("geoffair.org");

    // store the UNIX file as $HOME/.config/geoffair/Qt_OSM_Map.conf - nav: . hconf; cd geoffair
    QApplication::setOrganizationName("geoffair");
    QApplication::setApplicationName(APP_SETN);
    QApplication::setApplicationVersion(APP_SETV);

    QString title(APP_NAME);
    title.append(" v");
    title.append(APP_VERS);

    MainWindow w;
    w.show();
    
    return a.exec();
}
