#ifndef QRESIZABLEINNERWIDGET_H
#define QRESIZABLEINNERWIDGET_H

#include <QMouseEvent>
#include <QPoint>
#include <QWidget>
#include <QDebug>


class ResizableInnerWidget: public QWidget
{
    Q_OBJECT

    enum SelectionMode{ Left=1, Right=2, Top=4, Bottom=8, TL=5, TR=6, BL=9, BR=10, Move,None };
    SelectionMode _mode;
public:
    ResizableInnerWidget(QWidget *parent) : QWidget(parent),_beingResized(false){}
    virtual ~ResizableInnerWidget() {}

protected:
    bool _beingResized; //used to resize canvas widget and not allow drawing anything

    inline void mousePressEvent(QMouseEvent* event) override  {
        int d = 5;

        if (event->x() >= width() - d && event->y() >= height() - d) {
                _mode = BR;
               setCursor(Qt::SizeFDiagCursor);
           }  else if (event->x() >= width() - d) {
                _mode = Right;
               setCursor(Qt::SizeHorCursor);
           }  else if (event->y() >= height() - d) {
                _mode = Bottom;
               setCursor(Qt::SizeVerCursor);
           } else return;
        _beingResized = true;
    }
inline void mouseMoveEvent(QMouseEvent* event) override{

    if(!_beingResized) return;
    int right(0), bottom(0);

    QRect container(QPoint(0,0),geometry().size());
    if(_mode & Right){
        right = event->x() - container.width();
    }
    if(_mode & Bottom){
        bottom = event->y() - container.height();
    }
    container.adjust(0,0, right, bottom);
    if(container.height()<5 || container.width()<5) return;
    setGeometry(QRect(geometry().topLeft(), container.size()));
}

inline void mouseReleaseEvent(QMouseEvent *event) override{
    setCursor(Qt::ArrowCursor);
    _beingResized = false;
}

};
#endif // QRESIZABLEINNERWIDGET_H


