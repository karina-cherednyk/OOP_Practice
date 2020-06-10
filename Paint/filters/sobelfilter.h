#ifndef SOBELFILTER_H
#define SOBELFILTER_H
#include "afilter.h"
class QImage;
class SobelFilter:virtual public AFilter
{
public:
    SobelFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    QImage doFiltration(const QImage& img,double* tan);
};


#endif // SOBELFILTER_H
