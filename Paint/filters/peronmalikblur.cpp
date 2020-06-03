#include "peronmalikblur.h"
#include <QtGlobal>
#include<QColor>
#include <QImage>
#include <QHash>
QString PeronaMalikBlur::deltaName="Delta";
QString PeronaMalikBlur::blurLevelName ="BlurLevel";
QString PeronaMalikBlur::coefficientName="Coefficient";
PeronaMalikBlur::PeronaMalikBlur()
{
    params.append({blurLevelName , DOUBLE, 0,1000});
    params.append({ deltaName, DOUBLE, 0.01,10});
    params.append({ coefficientName, DOUBLE, 1,100});
}
/**
 * @brief PeronaMalikBlur::getBlurLevel
 * @return level of bluring
 */
const double& PeronaMalikBlur::getBlurLevel()const{
    return blurLevel;
}
/**
 * @brief PeronaMalikBlur::getDelta
 * @return step of bluring
 */
const double& PeronaMalikBlur::getDelta()const{
    return delta;
}
/**
 * @brief PeronaMalikBlur::getCoefficient
 * @return coefficient of bluring
 */
const double& PeronaMalikBlur::getCoefficient()const{
    return coefficient;
}
/**
 * @brief PeronaMalikBlur::setBlurLevel
 * @param bl - bluring level
 */
void PeronaMalikBlur::setBlurLevel(const double& bl){
    blurLevel=qMax(0.0,qMin(bl,1000.0));
}
/**
 * @brief PeronaMalikBlur::setDelta
 * @param d - step of bluring
 */
void PeronaMalikBlur::setDelta(const double& d){
    delta=qMax(0.01,qMin(d,10.0));
}

/**
 * @brief PeronaMalikBlur::setCoefficient
 * @param c - bluring coefficient
 */
void PeronaMalikBlur::setCoefficient(const double& c){
    coefficient=qMax(1.0,qMin(c,100.0));
}


/**
 * @brief PeronaMalikBlur::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return
 */
QImage PeronaMalikBlur::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    if(coeffs!=nullptr){
        setDelta(coeffs->value(deltaName));
        setBlurLevel(coeffs->value(blurLevelName));
        setCoefficient(coeffs->value(coefficientName));}
    QImage res(img);
    int sizeX=img.width()+2;
    int sizeY =img.height()+2;
    res.convertTo(QImage::Format_ARGB32);
    res.setAlphaChannel(img.alphaChannel());
    int ** red= new int* [sizeX];
    int ** green= new int* [sizeX];
    int ** blue= new int* [sizeX];
    doArray(red,sizeX,sizeY);
    doArray(green,sizeX,sizeY);
    doArray(blue,sizeX,sizeY);
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            red[j+1][i+1]=current.red();
            green[j+1][i+1]=current.green();
            blue[j+1][i+1]=current.blue();
        }
    }
    doBlur(red,sizeX,sizeY);
    doBlur(green,sizeX,sizeY);
    doBlur(blue,sizeX,sizeY);
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor current = res.pixel(j,i);
            current.setRed(qMax(0,qMin(255,red[j+1][i+1])));
            current.setGreen(qMax(0,qMin(255,green[j+1][i+1])));
            current.setBlue(qMax(0,qMin(255,blue[j+1][i+1])));
            res.setPixelColor(j, i,current);
        }
    }
    deleteArray(red,sizeX);
    deleteArray(green,sizeX);
    deleteArray(blue,sizeX);
    return res;
}
/**
 * @brief PeronaMalikBlur::doBlur
 * @param img - array of one channel (r,g,b) brightness
 * @param width - array sizeX
 * @param height - array sizeY
 * do bluring for one channel of image, new brightness will be in the array
 */
void PeronaMalikBlur::doBlur(int** img, int width, int height)const {
    int ** temp = new int* [width];
    doArray(temp,width,height);
    double north, south, east, west;
    double level=0;
    while(level<blurLevel){
        for(int i=1;i<height-1;++i){
            for(int j=1;j<width-1;++j){
                north = img[j-1][i]-img[j][i];
                south = img[j+1][i]-img[j][i];
                east = img[j][i-1]-img[j][i];
                west = img[j][i+1]-img[j][i];
                temp[j][i]=img[j][i]+delta*(g(north)*north+g(south)*south+g(east)*east+g(west)*west);
            }
        }
        level+=delta;
        for(int i=0;i<height;++i){
            for(int j=0;j<width;++j){
                img[j][i]=temp[j][i];
            }
        }

    }
    deleteArray(temp,width);
}
/**
 * @brief PeronaMalikBlur::g
 * @param x - brightness
 * @return norm of gradient
 */
double PeronaMalikBlur::g(const double& x)const{
    return 1/(1+(x/coefficient)*(x/coefficient));
}
/**
 * @brief PeronaMalikBlur::doArray
 * @param ar - array
 * @param sizeX
 * @param sizeY
 * create an two-dimensional int array [sizeX][sizeY] initialazed by 0
 */
void PeronaMalikBlur::doArray(int** ar, int sizeX, int sizeY) const{

    for (int i = 0; i < sizeX; ++i){
        ar[i] = new int [sizeY];
        for(int j=0;j<sizeY;++j)
            ar[i][j]=0;
    }

}
/**
 * @brief PeronaMalikBlur::deleteArray
 * @param ar - array for deleting
 * @param size - sizeX of array
 * called to delete two-dimensional array
 */
void PeronaMalikBlur::deleteArray(int** ar, int size) const{
    for (int i = 0; i <size; i++)
        delete []ar[i];
    delete []ar;

}
