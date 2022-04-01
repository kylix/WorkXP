#include "mainwindow.h"
#include "thememanager.h"
#include "preferences.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Preferences::instance();

    ThemeManager::instance();


    MainWindow w;



    w.showMaximized();
    w.init();

    w.newFile(true);

    return a.exec();
}
