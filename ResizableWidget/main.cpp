

#include "showsizewidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ShowSizeWidget wdg;
    wdg.show();
    return a.exec();
}
