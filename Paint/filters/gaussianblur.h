#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

#include "afilter.h"
class QColor;
class GaussianBlur:virtual public AFilter
{
public:
    GaussianBlur();
    virtual QImage doFiltration(const QImage&,const QHash<QString,double>* coeffs = nullptr);
    const double& getSigma()const;
    const int& getRadius()const;
    void setSigma(const double&);
    void setRadius(const int&);
private:
    static QString sigmaName;
    double sigma;//sigma 0.001-100
    static QString radiusName;
    int radius;//radius of blur mask 1-

};

#endif // GAUSSIANBLUR_H
