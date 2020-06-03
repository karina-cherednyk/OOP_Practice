#include "filterform.h"
#include "ui_filterform.h"
#include <QFormLayout>
#include <QIntValidator>
#include <QLineEdit>
#include <QDebug>


FilterForm::FilterForm( const QImage& im, QWidget *parent) :
    QWidget(parent,Qt::Window),_origin(im),_res(new QImage(im)),
    ui(new Ui::FilterForm)
{
    ui->setupUi(this);
    _form = ui->formLayout;
    _sample = ui->sampleWidget;
    _sample->setPointer(&_res);
    _sample->update();


//    for(const Parameter& p : _filter->getParams()){
//        QLabel* name = new QLabel(this);
//        QLineEdit* param = new QLineEdit(this);
//        if( p.type == INT) param->setValidator(intValidator(p.min,p.max));
//        else if(p.type == DOUBLE) param->setValidator(doubleValidator(p.min, p.max));

//        name->setText(p.name);
//        param->setText("5");
//        _form->addRow(name, param);
//    }
//    qDebug() << _form->children();
}



FilterForm::~FilterForm()
{
    delete _res;
//    delete  _filter;
    delete ui;
}

QDoubleValidator *FilterForm::doubleValidator(double min, double max)
{
    QDoubleValidator* v = new QDoubleValidator(this);
    v->setBottom(min);
    v->setTop(max);
    return v;
}

QIntValidator *FilterForm::intValidator(int min, int max)
{
    QIntValidator* v = new QIntValidator(this);
    v->setBottom(min);
    v->setTop(max);
    return v;
}

void FilterForm::on_tryButton_clicked()
{
//    if(_res) delete _res;
//    _res = new QImage(_filter->doFiltration(_origin));
}
