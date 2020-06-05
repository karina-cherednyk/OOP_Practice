#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile stylesheet_file(":/stylesheets/Aqua.qss");
    stylesheet_file.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(stylesheet_file.readAll());
    a.setStyleSheet(stylesheet);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
