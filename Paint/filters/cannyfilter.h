#ifndef CANNYFILTER_H
#define CANNYFILTER_H
#include "afilter.h"

class CannyFilter:virtual public AFilter
{
public:
    CannyFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getLow()const;
    void setLow(const int&);
private:
    static QString lowName;
    double low;//contrast coefficient 0-255
    int compare(int, int, int,const QImage&);
    int getX(int, int);
    int getY(int,int);
    int checkN(int,const QImage&);
};
#endif // CANNYFILTER_H
