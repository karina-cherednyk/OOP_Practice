#include "countmodel.h"
#include "delegate.h"
#include "nameswidget.h"
#include "ui_nameswidget.h"
#include <QFile>
#include <QStringListModel>
#include <QTextStream>



NamesWidget::NamesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NamesWidget)
{
    ui->setupUi(this);

    QListView* list = ui->list;





    list->setModel(new CountModel(this));

    ui->list->setItemDelegate(new ColorDelegate(Qt::green,this));

    list->setDragEnabled(true);
    list->setAcceptDrops(true);
    list->setDropIndicatorShown(true);
    list->setDefaultDropAction(Qt::MoveAction);
}


NamesWidget::NamesWidget(QString fileName, QWidget *parent):NamesWidget(parent)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    QStringList names = in.readAll().split('\n');

    ui->list->setModel(new CountModel(names,this));
    ui->list->setItemDelegate(new ColorDelegate(Qt::red,this));


}

NamesWidget::~NamesWidget()
{
    delete ui;
}
