#include "negativefilter.h"

#include "QImage"
#include<QColor>
NegativeFilter::NegativeFilter()
{

}
/**
 * @brief NegativeFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return negative of  image
 */
QImage NegativeFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            current.setRed(255-current.red());
            current.setGreen(255-current.green());
            current.setBlue(255-current.blue());
            res.setPixelColor(j, i,current);

        }
    }
    return res;
}


