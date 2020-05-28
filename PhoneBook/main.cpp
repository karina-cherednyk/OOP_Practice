#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

void show(QAbstractItemModel* m){
    for(int i=0;i < m->rowCount(); ++i)
        qDebug() << i <<')'<< m->data(m->index(i,0)).toString();

}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    ContactListModel* _model = new ContactListModel(nullptr, ":/contacts/contacts.txt");
    QSortFilterProxyModel* _proxy  = new QSortFilterProxyModel();
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(_model);

    _proxy->sort(0);

    show(_model);
    show(_proxy);

    return a.exec();
}
