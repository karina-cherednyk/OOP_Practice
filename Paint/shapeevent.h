#ifndef SHAPEEVENT_H
#define SHAPEEVENT_H
#include <QRect>
#include <QWidget>
class ShapeEvent
{

protected:


    QRect  _workingRect, _updateRect;
    QWidget* _widget;


   bool possiblySetSelectionMode(QPoint pos);
   inline void resizeWorkingRectTo(QPoint p){

       const QPoint& topLeft = _workingRect.topLeft();
       if(p.x()<topLeft.x() || p.y() < topLeft.y() ) return;
       _workingRect.setBottomRight(p);
    }


   inline void newWorkingRect(QPoint p){
        _workingRect = QRect(p,QSize(0,0));
   }

public:
    inline ShapeEvent(QWidget* w):_widget(w){}
    inline virtual void processPress(QPoint p){
        newWorkingRect(p);
    }
    inline virtual void processMove(QPoint p){
        _updateRect = _workingRect;

        resizeWorkingRectTo(p);
         //update
        (_updateRect |= _workingRect).adjust(-3, -3, +3, +3);
    }
    inline virtual void processRelease()
    {
         _widget->setCursor(Qt::ArrowCursor);
        //update
        _updateRect = _workingRect.adjusted(-3, -3, +3, +3);

    }


    inline const QRect& getWorkingRect(){
        return _workingRect;
    }


    inline void setSelectionRect(const QRect& r){
        _workingRect = r;
        _updateRect |= _workingRect;
    }

    inline const QRect& getUpdateRect(){
        return _updateRect;
    }



};

#endif // SHAPEEVENT_H
