#ifndef ROTATEEVENT_H
#define ROTATEEVENT_H

#include <QLine>
#include <QPoint>
#include <QRect>

/**
 * Used when rotationEvent is triggered in Canvas class
 *
 */
class RotateEvent
{

    QPoint _rotationCenter;
    QPoint _begin;
    QPoint _end;
    QRect _rotatingRect;
    QRect _updateRect;

    /**
     * calculates what area needs to be updated
     * based on rotation center point and radius of rotation circle inside updated rect
     * */
    static QRect getUpdateRect(const QPoint& center, const QRect& rect){

        //count distances between center and all 4 points of rotating rectangle
       QLineF tlRadius(center,rect.topLeft()),trRadius(center, rect.topRight()),
                blRadius(center,rect.bottomLeft()),brRadius(center, rect.bottomRight());

       //finds maximum distance
       double radius = qMax(qMax(tlRadius.length(), blRadius.length()), qMax(trRadius.length(), brRadius.length()));

       return QRect(center.x()-radius, center.y()-radius, 2*radius, 2*radius);
   }

public:


    /**
     * angle between vectors
     * [center, user pressed point] and [center, user moved point]
     */
    inline double angle(){
        QLineF one(_rotationCenter,_begin);
        QLineF two(_rotationCenter, _end);
        return two.angleTo(one);
    }

    /**
     * relative point of top left corner of rotating rect,
     * after translation of coordinates to rotating center was performed
     */
    inline QPoint getAfterTransformOriginPoint(){
        return QPoint(_rotatingRect.left()-_rotationCenter.x(),_rotatingRect.top() - _rotationCenter.y());
    }

    inline const QRect& getUpdateRect(){
        return _updateRect;
    }
    inline const QRect& getRotatingRect(){
        return _rotatingRect;
    }
    inline void setCenter(const QPoint& p) {
        _rotationCenter = p;
        _updateRect = getUpdateRect(_rotationCenter,_rotatingRect);
    }
    inline void setRotatingRect(const QRect& rect) {
        _rotatingRect = rect;
        _updateRect = getUpdateRect(_rotationCenter,_rotatingRect);
    }

    inline void setEnd(const QPoint& p) {_end=p;}
    inline void setBegin(const QPoint& p) {_begin=p;}
    inline QPoint& center(){ return _rotationCenter;}


};

#endif // ROTATEEVENT_H
