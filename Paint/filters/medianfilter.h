#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H
#include "afilter.h"

class MedianFilter:virtual public AFilter
{
public:
    MedianFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getRadius()const;
    void setRadius(const int&);
private:
    static QString radiusName;
    int radius;//0-10

};

#endif // MEDIANFILTER_H
