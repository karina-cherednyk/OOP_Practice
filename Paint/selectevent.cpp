
#include "selectevent.h"
#include <QDebug>

bool SelectEvent::possiblySetSelectionMode(QPoint pos)
{
    if(qAbs(pos.x()-_workingRect.right()) <5 && _workingRect.top()<= pos.y() && _workingRect.bottom() >= pos.y()){
        _widget->setCursor(Qt::SizeHorCursor);
        _mode = Right;
        return true;
    }
    else if(qAbs(pos.x()-_workingRect.left()) <5  && _workingRect.top()<= pos.y() && _workingRect.bottom() >= pos.y()){
         _widget->setCursor(Qt::SizeHorCursor);
        _mode = Left;
        return true;
    }
    else if(qAbs(pos.y()-_workingRect.bottom()) <5  && _workingRect.left()<= pos.x() && _workingRect.right() >= pos.x()){
        _widget->setCursor(Qt::SizeVerCursor);
        _mode = Bottom;
        return true;
    }
    else if(qAbs(pos.y()-_workingRect.top()) <5  && _workingRect.left()<= pos.x() && _workingRect.right() >= pos.x()){
         _widget->setCursor(Qt::SizeVerCursor);
        _mode = Top;
        return true;
    }
    else if(_workingRect.contains(pos)){
         _widget->setCursor(Qt::SizeAllCursor);
        _mode = Move;
        return true;
    }

    return false;
}


bool SelectEvent::possiblyResizeSelectionRect(QPoint pos)
{
     if(_mode == Right) {
        if(pos.x()<=_bufferedRect.left()) return false;
        _workingRect.setRight(pos.x());
    }
    else if(_mode == Left) {
         if(pos.x()>=_bufferedRect.right() ) return false;
        _workingRect.setLeft(pos.x());
    }
    else if(_mode == Top) {
         if(pos.y()>=_bufferedRect.bottom() ) return false;
        _workingRect.setTop(pos.y());
    }
    else if(_mode == Bottom) {
         if(pos.y()<=_bufferedRect.top() ) return false;
        _workingRect.setBottom(pos.y());

    }
    return true;
}

void SelectEvent::processPress(QPoint p){
    _selectionFinished = false;

    _lastPoint = p;

    if(hasSelection() && possiblySetSelectionMode(p))return;
     _isVisible = true;
    _mode = None;

    newWorkingRect(_lastPoint);
}


void SelectEvent::processMove(QPoint p)
{
        _updateRect = _workingRect;

        if(_mode == None)  resizeWorkingRectTo(p);
        else if(_mode == Move) moveWorkingRectTo(p);
        else if(!possiblyResizeSelectionRect(p)){
            return;
        };

         //update
        (_updateRect |= _workingRect).adjust(-3, -3, +3, +3);
}

void SelectEvent::processRelease(){
    _selectionFinished = true;
    _widget->setCursor(Qt::ArrowCursor);
   //update
   _updateRect = _workingRect.adjusted(-3, -3, +3, +3);
   _bufferedRect = _workingRect;
}
