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


    list->setDragEnabled(true);
    list->setAcceptDrops(true);
    list->setDropIndicatorShown(true);
    list->setDefaultDropAction(Qt::MoveAction);

    list->setModel(new QStringListModel(this));
}

NamesWidget::NamesWidget(QString fileName, QWidget *parent):NamesWidget(parent)
{
    QFile file(":/res/names.txt");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    QStringList names = in.readAll().split('\n');
    ui->list->setModel(new QStringListModel(names));
}

NamesWidget::~NamesWidget()
{
    delete ui;
}
