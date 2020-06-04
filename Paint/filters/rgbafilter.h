#ifndef RGBAFILTER_H
#define RGBAFILTER_H
#include "afilter.h"

class RGBAFilter:virtual public AFilter
{
public:
    RGBAFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getDeltaRed()const;
    const int& getDeltaGreen()const;
    const int& getDeltaBlue()const;
    const int& getDeltaAlpha()const;
    void setDeltaRed(const int&);
    void setDeltaGreen(const int&);
    void setDeltaBlue(const int&);
    void setDeltaAlpha(const int&);
private:
    static QString deltaRedName;
    static QString deltaGreenName;
    static QString deltaBlueName;
    static QString deltaAlphaName;
    int deltaRed;//red deviation -255-255
    int deltaBlue;//blue deviation -255-255
    int deltaGreen;//green deviation -255-255
    int deltaAlpha;//alpha deviation -255-255


};

#endif // RGBAFILTER_H
