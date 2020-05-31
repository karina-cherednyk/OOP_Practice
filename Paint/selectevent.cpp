
#include "selectevent.h"
#include <QDebug>

bool pass(int a, int b){
    return qAbs(a-b) < 5;
}
bool mid(int a, int m, int b){
    return a<=m && m<=b;
}
/**
 * does user want to transform selection?
 * if so, then assign appropriate mode
 */
bool SelectEvent::possiblySetSelectionMode(QPoint pos)
{
    const QRect& wr = _workingRect;
    if(pass(pos.x(),wr.right()) && mid(wr.top(),pos.y(),wr.bottom())){
        if(pass(pos.y(), wr.top())){
            _widget->setCursor(Qt::SizeBDiagCursor);
            _mode = TR;
        }
        else if(pass(pos.y(),wr.bottom())){
            _widget->setCursor(Qt::SizeFDiagCursor);
            _mode = TR;
        }
        else {
            _widget->setCursor(Qt::SizeHorCursor);
            _mode = Right;
        }
        return true;
    }
    else if(pass(pos.x(),wr.left()) && mid(wr.top(),pos.y(),wr.bottom())){
        if(pass(pos.y(), wr.top())){
            _widget->setCursor(Qt::SizeFDiagCursor);
            _mode = TL;
        }
        else if(pass(pos.y(),wr.bottom())){
            _widget->setCursor(Qt::SizeBDiagCursor);
            _mode = TL;
        }
        else {
            _widget->setCursor(Qt::SizeHorCursor);
            _mode = Left;
        }
        return true;
    }
    else if(pass(pos.y(),wr.bottom()) && mid(wr.left(),pos.x(),wr.right())){
        _widget->setCursor(Qt::SizeVerCursor);
        _mode = Bottom;
        return true;
    }
    else if(pass(pos.y(),wr.top()) && mid(wr.left(),pos.x(),wr.right())){
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

/**
 * try to resize rect according to mode
 */
bool SelectEvent::possiblyResizeSelectionRect(QPoint pos)
{
    if(_mode & Right) {
        if(pos.x()<=_bufferedRect.left()) return false;
        _workingRect.setRight(pos.x());
    }
    if(_mode & Left) {
         if(pos.x()>=_bufferedRect.right() ) return false;
        _workingRect.setLeft(pos.x());
    }
    if(_mode & Top) {
         if(pos.y()>=_bufferedRect.bottom() ) return false;
        _workingRect.setTop(pos.y());
    }
    if(_mode & Bottom) {
         if(pos.y()<=_bufferedRect.top() ) return false;
        _workingRect.setBottom(pos.y());

    }
    return true;
}

void SelectEvent::processPress(QPoint p){
    _selectionFinished = false;

    _lastPoint = p;
    //is selection rect visible? & user wants to interact with it
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
