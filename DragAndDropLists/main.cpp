
#include "nameswidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NamesWidget w1;
    NamesWidget w2(":/res/names.txt");
    w1.show();
    w2.show();

    return a.exec();
}
