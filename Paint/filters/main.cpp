#include <QApplication>
#include <QLabel>
#include <QHash>
#include "negativefilter.h"
#include "brightnessfilter.h"
#include "blackandwhitefilter.h"
#include "grayfilter.h"
#include "sepiafilter.h"
#include "contrastfilter.h"
#include "noisefilter.h"
#include "peronmalikblur.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QImage myImage;
    myImage.load("D:\\qt\\filters\\test.png");
    NegativeFilter f;
    //BrightnessFilter b;
    //NoiseFilter n;
    //BlackAndWhiteFilter bw;
    //b.setBrightness(2);
    //ContrastFilter cf;
    //GrayFilter g;
   // SepiaFilter s;
    //bw.setTreshold(125);
    //s.setDepth(1000);
   //cf.setCoefficient(10);
   //b.setBrightness(100);
    PeronaMalikBlur p;
    //n.setCoefficient(10);
   /* p.setDelta(0.1);
    p.setBlurLevel(10);
    p.setCoefficient(100);*/
    QHash<QString, double> list;

    list.insert("Delta",0.1);
    list.insert("BlurLevel",10);
    list.insert("Coefficient",8);
    QImage res=p.doFiltration(myImage,&list);
    QLabel myLabel1;

    myLabel1.setPixmap(QPixmap::fromImage(myImage));
    myLabel1.show();
    QLabel myLabel2;
    myLabel2.setStyleSheet("QLabel { background-color : red; color : blue; }");
    myLabel2.setPixmap(QPixmap::fromImage(res));
    myLabel2.show();
    //b.setBrightness(-200);
   // bw.setTreshold(200);
    //s.setDepth(4);
    //cf.setCoefficient(100);
   // n.setCoefficient(100);
    //QImage res2=p.doFiltration(myImage);
   // QLabel myLabel3;
   // myLabel3.setPixmap(QPixmap::fromImage(res2));
   // myLabel3.show();
   // MainWindow w;
   // w.show();
    return a.exec();
}
