#include "grayfilter.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>

GrayFilter::GrayFilter()
{

}
/**
 * @brief GrayFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return grayscale image
 */
QImage GrayFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            double grey = ( 0.2126*current.red()+ 0.7152*current.green() + 0.0722*current.blue());
            current.setRed(grey);
            current.setGreen(grey);
            current.setBlue(grey);
            res.setPixelColor(j, i,current);

        }
    }
    return res;

}

