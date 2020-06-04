#ifndef BLACKANDWHITEFILTER_H
#define BLACKANDWHITEFILTER_H

#include "afilter.h"
class BlackAndWhiteFilter:virtual public AFilter
{
public:
    BlackAndWhiteFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);
    const int& getTreshold()const;
    void setTreshold(const int& t);
private:
    static QString tresholdName;
    int treshold;//treshold of pixel brightness for being white 0-255


};


#endif // BLACKANDWHITEFILTER_H
