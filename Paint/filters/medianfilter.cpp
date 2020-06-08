#include "medianfilter.h"
#include <QImage>
#include <QColor>
#include <QHash>
#include <algorithm>
QString MedianFilter::radiusName="Radius";
MedianFilter::MedianFilter():AFilter("Median")
{
    params.append({ radiusName, INT, 0,5,0});
}
/**
 * @brief MedianFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return image after median filter
 */
QImage MedianFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    if(coeffs!=nullptr){
        setRadius(coeffs->value(radiusName));
    }
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    QColor current ;
    int a;
    int size =(radius*2+1)*(radius*2+1);
    int * red = new int[size];
    int * green = new int[(radius*2+1)*(radius*2+1)];
    int * blue= new int[(radius*2+1)*(radius*2+1)];
    for(int i=radius;i<res.height()-radius;++i){
        for(int j=radius;j<res.width()-radius;++j){
            a=0;
            for(int k =-radius;k<=radius;++k){
                for(int p =-radius;p<=radius;++p,a++){
                    current = img.pixel(j+k,i+p);
                    red[a]=current.red();
                    blue[a]=current.blue();
                    green[a]=current.green();
                }}
            std::sort (red, red+size);
            std::sort (green, green+size);
            std::sort (blue, blue+size);
            current.setRed(red[size/2]);
            current.setGreen(green[size/2]);
            current.setBlue(blue[size/2]);
            res.setPixelColor(j,i,current);

        }
    }
    res.setAlphaChannel(img.alphaChannel());
    delete [] red;
    delete [] green;
    delete [] blue;
    return res;
}
/**
 * @brief MedianFilter::getRadius
 * @return radius of mask
 */
const int& MedianFilter::getRadius()const{
    return radius;
}
/**
 * @brief MedianFilter::setRadius
 * @param r - new radius
 */
void MedianFilter::setRadius(const int& r){
    radius = qMax(0,qMin(5,r));
}

