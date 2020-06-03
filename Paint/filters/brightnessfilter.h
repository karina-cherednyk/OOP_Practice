#ifndef BRIGHTNESSFILTER_H
#define BRIGHTNESSFILTER_H
#include "afilter.h"
class BrightnessFilter:virtual public AFilter
{
public: BrightnessFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getBrightness()const;
    void setBrightness(const int& b);
private:
    static QString brightnessName;
    int brightness;//brightness deviation -255-255

};


#endif // BRIGHTNESSFILTER_H
