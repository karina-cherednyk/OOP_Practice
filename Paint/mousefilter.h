#ifndef MOUSEFILTER_H
#define MOUSEFILTER_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class MouseFilter :public QObject{
protected:
    virtual bool eventFilter(QObject*, QEvent*);
public:
    MouseFilter(QObject* ob = 0);
};

MouseFilter::MouseFilter(QObject* ob): QObject(ob){}


#endif // MOUSEFILTER_H
