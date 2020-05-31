#ifndef SELECTEVENT_H
#define SELECTEVENT_H

#include "shapeevent.h"

#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QDebug>

class SelectEvent: public ShapeEvent
{
    enum SelectionMode{ Left, Right, Top, Bottom, Move,None };
    QPoint _lastPoint;
    SelectionMode _mode;
    QRect _bufferedRect;
    QImage _bufferedImage;
    bool _selectionFinished, _isVisible;
    bool possiblyResizeSelectionRect(QPoint pos);
    bool possiblySetSelectionMode(QPoint pos);

public:
    inline SelectEvent(QWidget* w):ShapeEvent(w),_mode(None),_isVisible(false),_selectionFinished(false){}

    inline void moveWorkingRectTo(QPoint p){

        int dx = _bufferedRect.left() - _lastPoint.x();
        int dy = _bufferedRect.top()  - _lastPoint.y();

        _workingRect.moveTo(p.x()+dx, p.y()+dy);
    }

    inline const QImage getTransformedImage(){
        return _bufferedImage.scaled(_workingRect.size());
    }
    inline void setImage(const QImage& im){
        _bufferedImage = im;
    }

    inline bool isFinished(){
        return _selectionFinished;
    }
    inline void removeSelection(){
        _isVisible = false;
    }
    inline bool hasSelection(){
        return _isVisible;
    }
    inline const QRect& getBufferedRect(){
        return _bufferedRect;
    }

    inline bool isNoneMode(){
        return _mode == None;
    }
    inline void enablePaste(){
        _mode = Move;
    }
    void processRelease() override;
    void processMove(QPoint p) override;
    void processPress(QPoint p) override;
};


#endif // SELECTEVENT_H
