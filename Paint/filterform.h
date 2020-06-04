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
    const QString _name;

public:
    explicit FilterForm(const QString& name, AFilter* filt,const QImage& im, QWidget *parent = nullptr);
    ~FilterForm();
signals:
    void addImage(const QString&, const QImage&);
private slots:


    void on_tryButton_clicked();



    void on_cancelButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::FilterForm *ui;
    QDoubleValidator* doubleValidator(double min, double max) ;
    QIntValidator* intValidator(int min, int max);
};



#endif // FILTERFORM_H