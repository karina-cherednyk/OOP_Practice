#include "sepiafilter.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>
SepiaFilter::SepiaFilter()
{

}
/**
 * @brief SepiaFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return sepia image
 */
QImage SepiaFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            /* int middle = (current.red() + current.green()+ current.blue()) / 3;
               current.setRed(qMin(middle+2*depth,255));
                current.setGreen(qMin(middle+depth,255));
                current.setBlue(qMin(middle,255));*/

            current.setRed(qMin(0.393*current.red()+0.769*current.green()+0.189*current.blue(),255.0));
            current.setGreen(qMin(0.349*current.red()+0.686*current.green()+0.168*current.blue(),255.0));
            current.setBlue(qMin(0.272*current.red()+0.543*current.green()+0.131*current.blue(),255.0));
            res.setPixelColor(j, i,current);

        }
    }
    return res;
}
