#include "ColourPicker/colourpicker.h"
#include "ColorSpectrum.h"
#include "ui_colorpicker.h"
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QImage>
#include <iostream>
#include <QPainter>
#include <QRect>
#include <QDebug>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    ColorSpectrum* cs = ui->qw;
    ui->sample->setPalette(Qt::black);

    QObject::connect(cs,SIGNAL(colorSelected(QColor)),this ,SLOT(changeSample(QColor)));
    QObject::connect(cs,SIGNAL(colorSelected(QColor)),this ,SLOT(changeGradient(QColor)));

    //gradient
    QLinearGradient colorGradient = QLinearGradient(0, 0, width(), 0);
    colorGradient.setSpread(QGradient::RepeatSpread);
    colorGradient.setColorAt(0, QColor(255,255,255));
    colorGradient.setColorAt(0.2, Qt::red);
    colorGradient.setColorAt(0.55, Qt::black);
    QBrush colorGradiantBrush = QBrush(colorGradient);


    /*QLinearGradient colorGradient = QLinearGradient(0, 0, width(), 0);
    colorGradient.setSpread(QGradient::RepeatSpread);
    colorGradient.setColorAt(0, QColor(255,255,255));
    colorGradient.setColorAt(1, Qt::red);
    QBrush colorGradiantBrush = QBrush(colorGradient);

    QLinearGradient blackGradient = QLinearGradient(0, 0, 0, height());
    blackGradient.setSpread(QGradient::RepeatSpread);
    blackGradient.setColorAt(0, QColor(255,255,255,0));
    blackGradient.setColorAt(1, Qt::black);
    QBrush blackGradiantBrush = QBrush(blackGradient);
    */

    QPalette pal;
    pal.setBrush(QPalette::Window, colorGradiantBrush);
    //pal.setBrush(QPalette::Foreground, blackGradient);

    ColorSpectrum* gradient = ui->colorGradient;
    ui->colorGradient->setPalette(pal);

    QObject::connect(gradient,SIGNAL(colorSelected(QColor)),this ,SLOT(changeSample(QColor)));


    //transperancy
    QSlider *horizontalSlider = new QSlider(Qt::Horizontal);
    horizontalSlider->setRange(255, 0);
    horizontalSlider->setInvertedAppearance(true);
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::changeSample(QColor color)
{
        //change sample
        ui->sample->setPalette(QColor(color.red(),color.green(), color.blue(), curr_trans));

        //for transperancy
        curr_red = color.red();
        curr_green = color.green();
        curr_blue = color.blue();
}

void ColorPicker::changeGradient(QColor color){

    QLinearGradient colorGradient = QLinearGradient(0, 0, width(), 0);
    colorGradient.setSpread(QGradient::RepeatSpread);
    colorGradient.setColorAt(0, QColor(255,255,255));
    colorGradient.setColorAt(0.2, color);
    colorGradient.setColorAt(0.55, Qt::black);
    QBrush colorGradiantBrush = QBrush(colorGradient);
    QPalette pal;
    pal.setBrush(QPalette::Window, colorGradiantBrush);
    ui->colorGradient->setPalette(pal);
}

void ColorSpectrum::mousePressEvent(QMouseEvent * event){

    QPointF p = event->pos();
    std::cout <<"Position: "<<p.rx()<<", "<<p.ry()<<std::endl;

    QImage im = grab(QRect(QPoint(0,0),size())).toImage();
    QColor curr_color = im.pixelColor(p.x(), p.y());
    std::cout <<"RGB: "<<curr_color.red()<<" "<<curr_color.green()<<" "<<curr_color.blue()<<std::endl;

    emit colorSelected(curr_color);
}

void ColorPicker::on_horizontalSlider_valueChanged(int position)
{
    ui->sample->setPalette(QColor(curr_red, curr_green, curr_blue, position));
    curr_trans = position;
    std::cout <<"Transperancy: "<<curr_trans<<std::endl;
}
