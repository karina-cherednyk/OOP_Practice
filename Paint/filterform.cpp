
#include "filterform.h"
#include "ui_filterform.h"
#include <QFormLayout>
#include <QIntValidator>
#include <QLineEdit>
#include <QDebug>
#include <QSizePolicy>
#include <QString>

FilterForm::FilterForm( AFilter* filt, const QImage& im, QWidget *parent) :
    QDialog(parent),_origin(im),_res(new QImage(im)),_filter(filt),_name(filt->getFilterName()),
    ui(new Ui::FilterForm)
{
    ui->setupUi(this);
    _form = ui->formLayout;
    _sample = ui->sampleWidget;
    _sample->setPointer(&_res);
    _sample->update();
    ui->filterName->setText(_name);
    QSizePolicy pol(QSizePolicy::Expanding, QSizePolicy::Fixed);

    for(const Parameter& p : _filter->getParams()){
        QLabel* name = new QLabel(this);
        QLineEdit* param = new QLineEdit(this);
        if( p.type == INT) param->setValidator(intValidator(p.min,p.max));
        else if(p.type == DOUBLE) param->setValidator(doubleValidator(p.min, p.max));

        name->setText(p.name);

        pol.setHorizontalStretch(1);
        name->setSizePolicy(pol);
        param->setSizePolicy(pol);
        param->setText(QString::number(p.def));
        //param->setMaxLength(5);
        param->setObjectName(p.name);
        _form->addRow(name, param);_form->itemAt(0,  QFormLayout::FieldRole)->widget()->objectName();
    }


}



FilterForm::~FilterForm()
{
    delete _res;
    delete  _filter;
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
    QHash<QString, double>* coeffs = new QHash<QString, double>;
    QLineEdit* edit;
    for(int i=0; i<_form->rowCount(); i++){
        edit = qobject_cast<QLineEdit*> (_form->itemAt(0,  QFormLayout::FieldRole)->widget());
        coeffs->insert(edit->objectName(), edit->text().toDouble());
    }
    if(_res) delete _res;
    _res = new QImage(_filter->doFiltration(_origin, coeffs));
    _sample->update();
}

void FilterForm::on_cancelButton_clicked()
{
    close();
}

void FilterForm::on_confirmButton_clicked()
{
    emit addImage(_name, *_res);
    close();
}
