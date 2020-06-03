#include "blackandwhitefilter.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QHash>
QString BlackAndWhiteFilter::tresholdName ="Treshold";
BlackAndWhiteFilter::BlackAndWhiteFilter()
{
    params.append({ tresholdName, INT, 0,255 });

}
/**
 * @brief BlackAndWhiteFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return black and white image
 */
QImage BlackAndWhiteFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    setTreshold(coeffs->value(tresholdName));
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            int total = (current.red()+current.green()+current.blue())/3;
            if(total>=treshold){
                current.setRed(255);
                current.setGreen(255);
                current.setBlue(255);

            }else{
                current.setRed(0);
                current.setGreen(0);
                current.setBlue(0);
            }
            res.setPixelColor(j, i,current);
        }
    }
    return res;
}
/**
 * @brief BlackAndWhiteFilter::getTreshold
 * @return brightness treshold
 */
const int& BlackAndWhiteFilter::getTreshold()const{
    return treshold;
}
/**
 * @brief BlackAndWhiteFilter::setTreshold
 * @param t- brightness treshold
 * pixels with average brightness less than treshold will be black, others - white
 */
void BlackAndWhiteFilter::setTreshold(const int& t){
    treshold=qMax(0,qMin(t,255));
}
