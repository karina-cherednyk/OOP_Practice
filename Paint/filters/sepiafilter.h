#ifndef SEPIAFILTER_H
#define SEPIAFILTER_H

#include "afilter.h"
class SepiaFilter:virtual public AFilter
{
public:
     SepiaFilter();
    virtual QImage doFiltration(const QImage& img,const QHash<QString,double>* coeffs = nullptr);

    /* const int& getDepth()const{
         return depth;
     }
     void setDepth(const int& t){
         depth=t;
     }
private:
     int depth;*/
};


#endif // SEPIAFILTER_H
