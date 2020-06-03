#include "contrastfilter.h"

#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QHash>
QString ContrastFilter::coefficientName ="Coefficient";
ContrastFilter::ContrastFilter()
{
    params.append({ coefficientName, DOUBLE, 0,100});
}
/**
     * @brief ContrastFilter::doFiltration
     * @param img - image for filtration
     * @param coeffs - arguments which are used for filtering
     * @return image with changed contrast level
     */
QImage ContrastFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    setCoefficient(coeffs->value(coefficientName));
    QImage res(img);
    int palette[255]={};
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    int avg =countAverage(res);
    for(int i =0;i<256;++i){
        palette[i]=qMax(0,qMin(255,(int)(avg+coefficient*(i-avg))));
    }
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            current.setRed(palette[current.red()]);
            current.setGreen(palette[current.green()]);
            current.setBlue(palette[current.blue()]);
            res.setPixelColor(j, i,current);

        }
    }
    return res;

}
/**
 * @brief ContrastFilter::getCoefficient
 * @return contrast coefficient
 */
const double& ContrastFilter::getCoefficient()const{
    return coefficient;
}

/**
 * @brief ContrastFilter::setCoefficient
 * @param c - new contrast coefficient
 * if c(0,1) - image become less contrast
 */
void ContrastFilter::setCoefficient(const double& c){
    coefficient=qMax(0.0,qMin(c,255.0));
}

/**
 * @brief ContrastFilter::countAverage
 * @param img - image for filtration
 * @return count average brightness of whole image
 */
int ContrastFilter::countAverage(const QImage& img)const{
    double res =0;
    for(int i=0;i<img.height();++i){
        for(int j=0;j<img.width();++j){
            QColor current = img.pixel(j,i);
            res +=current.red() * 0.299 + current.green() * 0.587 + current.blue() * 0.114;
        }
    }
    res/=(img.height()*img.width());
    return res;
};

