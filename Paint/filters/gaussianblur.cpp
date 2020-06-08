#include "gaussianblur.h"
#include "blackandwhitefilter.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QHash>
#include <QtMath>

QString GaussianBlur::radiusName="Radius";
QString GaussianBlur::sigmaName="Sigma";
GaussianBlur::GaussianBlur(): AFilter("Gaussian Blur")
{
    params.append({ radiusName, INT, 1,10, 2});
    params.append({ sigmaName, DOUBLE, 0.00001,100, 0.73876});
}
/**
 * @brief GaussianBlur::doFiltration
     * @param img - image for filtration
     * @param coeffs - arguments which are used for filtering
     * @return image with changed blur level
 */
QImage GaussianBlur::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    if(coeffs!=nullptr){
        setRadius(coeffs->value(radiusName));
        setSigma(coeffs->value(sigmaName));
    }
    QImage res(img);
    res.convertTo(QImage::Format_ARGB32);
    double*  mask=new double[(radius*2+1)*(radius*2+1)];
    int a=0;
    double constant=0;
    for(int i = -radius; i<=radius;i++){
        for(int k = -radius; k<=radius;k++,a++){
            mask[a]=exp(-(k*k+i*i)/(2*sigma*sigma))/(2*sigma*M_PI*sigma);

        }
    }
    double red;
    double blue;
    double green;
    QColor current;
    for(int i=radius;i<res.height()-radius;++i){
        for(int j=radius;j<res.width()-radius;++j){
            a = 0;
            red = 0;
            blue = 0;
            green = 0;
            constant=0;
            for(int k =-radius;k<=radius;++k){
                for(int p =-radius;p<=radius;++p,a++){
                    current = img.pixel(j+k,i+p);
                    red+=mask[a]*current.red();
                    blue+=mask[a]*current.blue();
                    green+=mask[a]*current.green();
                    constant+=mask[a];
                }}
            current.setRed(red/constant);
            current.setGreen(green/constant);
            current.setBlue(blue/constant);
            res.setPixelColor(j,i,current);

        }
    }
    res.setAlphaChannel(img.alphaChannel());
    delete []mask;
    return res;
}

/**
 * @brief GaussianBlur::getSigma
 * @return sigma
 */
const double& GaussianBlur::getSigma()const{
    return sigma;
}
/**
 * @brief GaussianBlur::getRadius
 * @return radius of mask
 */
const int& GaussianBlur::getRadius()const{
    return radius;
}
/**
 * @brief GaussianBlur::setSigma
 * @param s - new sigma
 */
void GaussianBlur::setSigma(const double& s){
    sigma=qMin(100.0,qMax(s,0.00001));
}
/**
 * @brief GaussianBlur::setRadius
 * @param r - new radius
 */
void GaussianBlur::setRadius(const int& r){
    radius=qMin(10, qMax(1,r));
}
