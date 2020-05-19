#include "showsizewidget.h"
#include "ui_showsizewidget.h"

#include <QResizeEvent>

ShowSizeWidget::ShowSizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowSizeWidget)
{
    ui->setupUi(this);

}

ShowSizeWidget::~ShowSizeWidget()
{
    delete ui;
}

void ShowSizeWidget::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    ui->label->setText(QString("width: %0, height: %1").arg(size.width()).arg(size.height()));
}
