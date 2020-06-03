#ifndef SAMPLEWIDGET_H
#define SAMPLEWIDGET_H

#include <QImage>
#include <QPainter>
#include <QWidget>

class SampleWidget: public QWidget {
 QImage** _pointer;
public:
    SampleWidget( QWidget *parent = nullptr):QWidget(parent),_pointer(nullptr){

    }

    inline void setPointer(QImage** p){ _pointer = p;}
    inline void paintEvent(QPaintEvent *event) override{
        QPainter p(this);
        if(_pointer){
            p.drawImage(QPoint(0,0),(**_pointer).scaled(size(),Qt::IgnoreAspectRatio));
        }
        QWidget::paintEvent(event);
    }
};
#endif // SAMPLEWIDGET_H
