#ifndef SELECTEVENT_H
#define SELECTEVENT_H

#include "shapeevent.h"

#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QDebug>

class SelectEvent: public ShapeEvent
{
    enum SelectionMode{ Left=1, Right=2, Top=4, Bottom=8, TL=5, TR=6, BL=9, BR=10, Move,None }; //T - Top, B - Bottom e.t.c.
    QPoint _lastPoint;
    SelectionMode _mode;
    QRect _bufferedRect;
    QImage _bufferedImage;
    bool _selectionFinished, _isVisible;
    bool possiblyResizeSelectionRect(QPoint pos);
    bool possiblySetSelectionMode(QPoint pos);

public:
    inline SelectEvent(QWidget* w):ShapeEvent(w),_mode(None),_isVisible(false),_selectionFinished(false){}

    /**
     * move selected area relatively to cursor
     */
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
    /**
     * was area selected (do you need to select rect first or now you have to trasnform it )
     */
    inline bool isFinished(){
        return _selectionFinished;
    }
    /**
     * used when Select button is toggled
     */
    inline void removeSelection(){
        _isVisible = false;
    }
    /**
     * will selected rect be displayed on canvas
     */
    inline bool hasSelection(){
        return _isVisible;
    }
    inline const QRect& getBufferedRect(){
        return _bufferedRect;
    }
    /**
     * what step is it? (true - select rect; false - transform it)
     */
    inline bool isNoneMode(){
        return _mode == None;
    }
    /**
     * as default mode is None (modified image not shown),
     * sets mode to smthn else to make image be drawn on canvas
     */
    inline void setAdjustMode(){
        _isVisible = true;
        _mode = Move;
    }
    /**
     * select area programmatically
     */
    inline void setSelectionRect(const QRect& r){
        _bufferedRect = r;
        _workingRect = r;
        if(_updateRect.isNull()) _updateRect = _workingRect;
        else _updateRect |= _workingRect;
        _updateRect.adjust(-3,-3,3,3);
    }

    void processRelease() override;
    void processMove(QPoint p) override;
    void processPress(QPoint p) override;
};


#endif // SELECTEVENT_H
