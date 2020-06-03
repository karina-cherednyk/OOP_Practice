#ifndef GRAYFILTER_H
#define GRAYFILTER_H
#include "afilter.h"
class GrayFilter:virtual public AFilter
{
public:
    GrayFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
};


#endif // GRAYFILTER_H
