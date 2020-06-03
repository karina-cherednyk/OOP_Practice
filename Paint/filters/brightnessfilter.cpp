#include "brightnessfilter.h"

#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QHash>
QString BrightnessFilter::brightnessName = "Brightness";
BrightnessFilter::BrightnessFilter()
{
    params.append({ brightnessName, INT, -255,255 });
}
/**
 * @brief BrightnessFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return image with new brightness
 */
QImage BrightnessFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs) {
    setBrightness(coeffs->value(brightnessName));
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            current.setRed((int)qMin(255, qMax(0, brightness+current.red())));
            current.setGreen((int)qMin(255, qMax(0, brightness+current.green())));
            current.setBlue((int)qMin(255, qMax(0, brightness+current.blue())));
            res.setPixelColor(j, i,current);

        }
    }
    return res;

}
/**
 * @brief BrightnessFilter::getBrightness
 * @return current brightness deviation
 */
const int& BrightnessFilter::getBrightness()const{
    return brightness;
}
/**
 * @brief BrightnessFilter::setBrightness
 * @param b - new brightness deviation
 */
void BrightnessFilter::setBrightness(const int& b){
    brightness=qMin(255,qMax(b,-255));
}

