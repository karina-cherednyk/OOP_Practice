#include "colorpicker.h"
#include "ui_colorpicker.h"
#include <QMouseEvent>
#include <QWidget>
#include <QImage>
#include <iostream>
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
    colorGradient.setSpread(QGradient::ReflectSpread);
    colorGradient.setColorAt(0, QColor(255,255,255));
    colorGradient.setColorAt(0.2, Qt::red);
    colorGradient.setColorAt(0.55, Qt::black);
    QBrush colorGradiantBrush = QBrush(colorGradient);


    QPalette pal;
    pal.setBrush(QPalette::Window, colorGradiantBrush);

    ColorSpectrum* gradient = ui->colorGradient;
    ui->colorGradient->setPalette(pal);

    QObject::connect(gradient,SIGNAL(colorSelected(QColor)),this ,SLOT(changeSample(QColor)));

}


void ColorPicker::changeSample(QColor color)
{
    QColor c(color.red(),color.green(), color.blue(), curr_trans);
        //change sample
        ui->sample->setPalette(c);

        //for transperancy
        curr_red = color.red();
        curr_green = color.green();
        curr_blue = color.blue();

        emit colorSet(c);
}

void ColorPicker::setColor(QColor color){
    ui->sample->setPalette(color);

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

    QImage im = grab(QRect(QPoint(0,0),size())).toImage();
    QColor curr_color = im.pixelColor(p.x(), p.y());


    emit colorSelected(curr_color);
}


void ColorPicker::on_horizontalSlider_valueChanged(int position)
{
    QColor c(curr_red, curr_green, curr_blue, position);
    ui->sample->setPalette(c);
    curr_trans = position;
    emit colorSet(c);
}


ColorPicker::~ColorPicker()
{
    delete ui;
}

