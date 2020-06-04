#include "noisefilter.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QRandomGenerator>
#include <QHash>
QString NoiseFilter::coefficientName ="Coefficient";
NoiseFilter::NoiseFilter(): AFilter("Noise")
{
  params.append({ coefficientName, INT, 0,255,0});
}
/**
 * @brief NoiseFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return image with random created noises
 */
QImage NoiseFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    if(coeffs!=nullptr)
        setCoefficient(coeffs->value(coefficientName));
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            int rand = QRandomGenerator::global()->bounded(-coefficient,coefficient);
            current.setRed(qMin(255, qMax(0, current.red()+rand)));
            current.setGreen(qMin(255, qMax(0, current.green()+rand)));
            current.setBlue(qMin(255, qMax(0, current.blue()+rand)));
            res.setPixelColor(j, i,current);

        }
    }
    res.setAlphaChannel(img.alphaChannel());
    return res;

}
/**
 * @brief NoiseFilter::getCoefficient
 * @return noise coefficient
 */
const int& NoiseFilter::getCoefficient()const{
    return coefficient;
}
/**
 * @brief NoiseFilter::setCoefficient
 * @param n - new noise coefficient
 * to image will be added value between -n and n
 */
void NoiseFilter::setCoefficient(const int& n){
    coefficient=qMax(0,qMin(255,n));
}
