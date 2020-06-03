#ifndef FILTERFORM_H
#define FILTERFORM_H

#include "samplewidget.h"

#include <QWidget>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QFormLayout>
#include <filters/afilter.h>
#include <QPainter>

namespace Ui {
class FilterForm;
}

class FilterForm : public QWidget
{
    Q_OBJECT
    AFilter* _filter;
    const QImage _origin;
    QImage* _res;
    SampleWidget* _sample;
    QFormLayout* _form;

public:
    explicit FilterForm(const QImage& im, QWidget *parent = nullptr);
    ~FilterForm();

private slots:


    void on_tryButton_clicked();

private:
    Ui::FilterForm *ui;
    QDoubleValidator* doubleValidator(double min, double max) ;
    QIntValidator* intValidator(int min, int max);
};



#endif // FILTERFORM_H
