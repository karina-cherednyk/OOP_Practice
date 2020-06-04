#ifndef NOISEFILTER_H
#define NOISEFILTER_H
#include "afilter.h"

class NoiseFilter:virtual public AFilter
{
public:
    NoiseFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getCoefficient()const;
    void setCoefficient(const int& b);
private:
    static QString coefficientName;
    int coefficient;//noise coefficient 0-255
};

#endif // NOISEFILTER_H
