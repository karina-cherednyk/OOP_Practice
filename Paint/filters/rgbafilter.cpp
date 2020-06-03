#include "rgbafilter.h"
#include <QImage>
#include <QHash>
QString RGBAFilter::deltaRedName = "Red";
QString RGBAFilter::deltaGreenName ="Green";
QString RGBAFilter::deltaBlueName ="Blue";
QString RGBAFilter::deltaAlphaName ="Alpha";
RGBAFilter::RGBAFilter()
{
    params.append({ deltaRedName, INT, -255,255});
    params.append({ deltaGreenName, INT, -255,255});
    params.append({ deltaBlueName, INT, -255,255});
    params.append({ deltaAlphaName, INT, -255,255});

}
/**
 * @brief RGBAFilter::getDeltaRed
 * @return red channel deviation
 */
const int& RGBAFilter::getDeltaRed()const{
    return deltaRed;
}
/**
 * @brief RGBAFilter::getDeltaGreen
 * @return green channel deviation
 */
const int& RGBAFilter::getDeltaGreen()const{
    return deltaGreen;
}
/**
 * @brief RGBAFilter::getDeltaBlue
 * @return blue channel deviation
 */
const int& RGBAFilter::getDeltaBlue()const{
    return deltaBlue;
}
/**
 * @brief RGBAFilter::getDeltaAlpha
 * @return alpha channel deviation
 */
const int& RGBAFilter::getDeltaAlpha()const{
    return deltaAlpha;
}
/**
 * @brief RGBAFilter::setDeltaRed
 * @param d - set red channel deviation
 */
void RGBAFilter::setDeltaRed(const int& d){
    deltaRed = qMax(-255, qMin(255,d));
}
/**
 * @brief RGBAFilter::setDeltaGreen
 * @param d - set green channel deviation
 */
void RGBAFilter::setDeltaGreen(const int& d){
    deltaGreen = qMax(-255, qMin(255,d));
}
/**
 * @brief RGBAFilter::setDeltaBlue
 * @param d - set blue channel deviation
 */
void RGBAFilter::setDeltaBlue(const int& d){
    deltaBlue = qMax(-255, qMin(255,d));
}
/**
 * @brief RGBAFilter::setDeltaAlpha
 * @param d - set alpha channel deviation
 */
void RGBAFilter::setDeltaAlpha(const int& d){
    deltaAlpha = qMax(-255, qMin(255,d));
}
/**
 * @brief RGBAFilter::doFiltration
 * @param img- image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return image with changed channels brightness and transparent
 */
QImage RGBAFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    setDeltaRed(coeffs->value(deltaRedName));
    setDeltaGreen(coeffs->value(deltaGreenName));
    setDeltaBlue(coeffs->value(deltaBlueName));
    setDeltaAlpha(coeffs->value(deltaAlphaName));
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            current.setRed(qMin(255, qMax(0, current.red()+deltaRed)));
            current.setGreen(qMin(255, qMax(0, current.green()+deltaGreen)));
            current.setBlue(qMin(255, qMax(0, current.blue()+deltaBlue)));
            current.setAlpha(qMin(255, qMax(0, current.alpha()+deltaAlpha)));
            res.setPixelColor(j, i,current);

        }
    }
    return res;

}
