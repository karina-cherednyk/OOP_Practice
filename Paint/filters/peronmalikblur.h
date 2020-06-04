#ifndef PERONAMALIKBLUR_H
#define PERONAMALIKBLUR_H

#include "afilter.h"

class PeronaMalikBlur:virtual public AFilter
{
public:
    PeronaMalikBlur();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const double& getBlurLevel()const;
    const double& getDelta()const;
    const double& getCoefficient()const;
    void setBlurLevel(const double&);
    void setDelta(const double&);
    void setCoefficient(const double&);
private:
    static QString deltaName;
    static QString blurLevelName;
    static QString coefficientName;
    double delta;//blur step 0.01-10
    double blurLevel;//blur level 0-1000
    double coefficient;//blur coefficient 1-100
    void doBlur(int** img, int width, int height)const;
    double g(const double& x)const;
    void doArray(int** ar, int sizeX, int sizeY) const;
    void deleteArray(int** ar, int size) const;
};
#endif // PERONAMALIKBLUR_H
