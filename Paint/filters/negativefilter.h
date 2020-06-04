#ifndef NEGATIVEFILTER_H
#define NEGATIVEFILTER_H
#include "afilter.h"
class NegativeFilter:virtual public AFilter
{
public:
    NegativeFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);

};

#endif // NEGATIVEFILTER_H
