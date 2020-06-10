#include "cannyfilter.h"
#include "gaussianblur.h"
#include "sobelfilter.h"
#include <QImage>
#include <QtMath>
#include <QHash>
QString CannyFilter::lowName="Low Border";
CannyFilter::CannyFilter():AFilter("Canny")
{
params.append({ lowName, INT, 0,255, 20});
}
/**
 * @brief CannyFilter::getX
 * @param a  - coord in 1D
 * @param width - width of image
 * @return x coord forom 1D to 2D
 */
int CannyFilter::getX(int a, int width){
    return a%width;
}
/**
 * @brief CannyFilter::getY
 * @param a  - coord in 1D
 * @param width - width of image
 * @return y coord forom 1D to 2D
 */
int CannyFilter::getY(int a,int width){
    return a/width;
}
/**
 * @brief CannyFilter::compare
 * @param i - coord 1D
 * @param a - coord 1D
 * @param b - coord 1D
 * @param img - image for comparing pixels
 * @return brightness of pixel
 */
int CannyFilter::compare(int i, int a, int b,const QImage& img){
    int w = img.width();
    if(((QColor)img.pixel(getX(a,w),getY(a,w))).red()>((QColor)img.pixel(getX(i,w),getY(i,w))).red()
            ||((QColor)img.pixel(getX(b,w),getY(b,w))).red()>((QColor)img.pixel(getX(i,w),getY(i,w))).red())
        return 0;
    return ((QColor)img.pixel(getX(i,w),getY(i,w))).red();
}
/**
 * @brief CannyFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return image conturs
 */
QImage CannyFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs){
    if(coeffs!=nullptr)
        setLow(coeffs->value(lowName));
    double* tan=new double[img.width()*img.height()];
    GaussianBlur gb;
    gb.setSigma(1);
    gb.setRadius(3);
    QImage res = SobelFilter().doFiltration(gb.doFiltration(img),tan);
    res.convertTo(QImage::Format_ARGB32);
    double cur_t;
    int a =0;
    int col=0;
    int w = img.width();
    int h = img.height();
    for(int i =w+1;i<h*(w-1);++i){
        cur_t=tan[i];
        if(cur_t<=-67.5||cur_t>=67.5){
            col =compare(i,i-w,i+w,res);
        }else if(cur_t<=-22.5){
            col =compare(i,i-w-1,i+w+1,res);
        }else  if(cur_t<=22.5){
            col = compare(i,i+1,i-1,res);
        }else{
            col = compare(i,i-w+1,i+w-1,res);
        }
        QColor c = res.pixel(getX(i,w),getY(i,w));
        c.setRed(col);
        c.setGreen(col);
        c.setBlue(col);
        res.setPixelColor(getX(i,w),getY(i,w),c);
        if((i+1)%img.width()==0)i+=1;


    }
    for(int i=0;i<res.height();++i){
        for(int j=0;j<res.width();++j){
            QColor cur = res.pixel(j,i);
            if(cur.red()<low){
                cur.setRed(0);
                cur.setGreen(0);
                cur.setBlue(0);
                res.setPixelColor(j,i,cur);
            }
        }
    }

    for(int i = res.width()+1;i<res.height()*(res.width()-1);++i){
        QColor c = res.pixel(getX(i,w),getY(i,w));
        if(c.red()!=0){
            int p=checkN(i,res);
            c.setRed(p);
            c.setGreen(p);
            c.setBlue(p);
            res.setPixelColor(getX(i,w),getY(i,w),c);
        }
        if((i+1)%res.width()==0)i+=1;
    }
    res.setAlphaChannel(img.alphaChannel());
    delete [] tan;
    return res;
}
/**
 * @brief CannyFilter::checkN
 * @param i - coord 1D
 * @param img - image
 * @return new brightness meaning
 */
int CannyFilter::checkN(int i,const QImage& img){
    int k = getX(i,img.width());
    int j=getY(i,img.width());
    if(((QColor)img.pixel(k,j+1)).red()!=0||((QColor)img.pixel(k,j-1)).red()!=0||
            ((QColor)img.pixel(k-1,j-1)).red()!=0||((QColor)img.pixel(k-1,j+1)).red()!=0||
            ((QColor)img.pixel(k-1,j)).red()!=0||((QColor)img.pixel(k+1,j-1)).red()!=0||
            ((QColor)img.pixel(k+1,j)).red()!=0||((QColor)img.pixel(k+1,j+1)).red()!=0) return ((QColor)img.pixel(k,j)).red();
    return 0;
}
/**
 * @brief CannyFilter::getLow
 * @return low border of brightness
 */
const int& CannyFilter::getLow()const{
    return low;
}
/**
 * @brief CannyFilter::setLow
 * @param l - new low border of brightness
 */
void CannyFilter::setLow(const int& l){
    low=qMax(0,qMin(255,l));

}
