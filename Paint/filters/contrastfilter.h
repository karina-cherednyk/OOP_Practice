#ifndef CONTRASTFILTER_H
#define CONTRASTFILTER_H
#include "afilter.h"
class ContrastFilter:virtual public AFilter
{
public:
    ContrastFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const double& getCoefficient()const;
    void setCoefficient(const double& b);
private:
    static QString coefficientName;
    double coefficient;//contrast coefficient 0-100
    int countAverage(const QImage& img)const;
};



#endif // CONTRASTFILTER_H
