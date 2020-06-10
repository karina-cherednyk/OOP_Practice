#include "sobelfilter.h"
#include "grayfilter.h"
#include <QImage>
#include <QtGlobal>
#include <QtMath>
SobelFilter::SobelFilter(): AFilter("Sobel")
{

}
/**
 * @brief SobelFilter::doFiltration
 * @param img - image for filtration
 * @param coeffs - arguments which are used for filtering
 * @return sobel image
 */
QImage SobelFilter::doFiltration(const QImage& img,const QHash<QString,double>* coeffs ){
    QImage res=GrayFilter().doFiltration(img);
    res.convertTo(QImage::Format_ARGB32);
    double mask1 [9]={-1,-2,-1,0,0,0,1,2,1};
    double mask2 [9]={-1,0,1,-2,0,2,-1,0,1};
    double _res;
    double res_x;
    double res_y;
    int a;
    QColor current;
    for(int i=1;i<res.height()-1;++i){
        for(int j=1;j<res.width()-1;++j){
            res_x=0;
            res_y=0;
            _res=0;
            a=0;
            for(int k =-1;k<=1;++k)
                for(int p = -1;p<=1;++p,++a){
                    current=img.pixel(p+j,i+k);
                    res_x+=mask1[a]*current.red();
                    res_y+=mask2[a]*current.red();
                }
            res_x*=0.25;
            res_y*=0.25;
            _res=abs(res_x)+abs(res_y);
            current.setRed( qMin(255.0,round(_res)));
            current.setGreen( qMin(255.0,round(_res)));
            current.setBlue( qMin(255.0,round(_res)));
            res.setPixelColor(j, i,current);
        }
    }
    res.setAlphaChannel(img.alphaChannel());
    return res;
}

/**
 * @brief SobelFilter::doFiltration
 * @param img - image for filtration
 * @param tan - pointer to tan array
 * @return sobel image
 */
QImage SobelFilter::doFiltration(const QImage& img,double* tan){
    QImage res=GrayFilter().doFiltration(img);
    res.convertTo(QImage::Format_ARGB32);
    double mask1 [9]={-1,-2,-1,0,0,0,1,2,1};
    double mask2 [9]={-1,0,1,-2,0,2,-1,0,1};
    double _res;
    double res_x;
    double res_y;
    int a;
    QColor current;
    for(int i=1;i<res.height()-1;++i){
        for(int j=1;j<res.width()-1;++j){
            res_x=0;
            res_y=0;
            _res=0;
            a=0;
            for(int k =-1;k<=1;++k)
                for(int p = -1;p<=1;++p,++a){
                    current=img.pixel(p+j,i+k);
                    res_x+=mask1[a]*current.red();
                    res_y+=mask2[a]*current.red();
                }
            res_x*=0.25;
            res_y*=0.25;
            if(res_x==0&&res_y==0){
                tan[i]=atan(0);
            }else{
               tan[i]=(round(atan2(res_y,res_x)*4/M_PI)*M_PI/4-M_PI/2)*180/M_PI;
            }
            _res=abs(res_x)+abs(res_y);
            current.setRed( qMin(255.0,round(_res)));
            current.setGreen( qMin(255.0,round(_res)));
            current.setBlue( qMin(255.0,round(_res)));
            res.setPixelColor(j, i,current);
        }
    }
    res.setAlphaChannel(img.alphaChannel());
    return res;
}
