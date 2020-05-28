#include "layermodel.h"

#include <QPainter>

LayerModel::LayerModel(QObject *parent)
    : QAbstractListModel(parent),_layers(nullptr)
{
}

int LayerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !_layers)
        return 0;
    return _layers->size();

}

QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QPair<QString,QImage> pair(_layers->operator[](index.row()).operator QPair<QString,QImage>());
    return QVariant::fromValue<QPair<QString,QImage>>(pair);
}



Delegate::Delegate(QObject *p):QItemDelegate(p){}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QString,QImage> l = index.data().value<QPair<QString,QImage>>();

    QRect rect = option.rect;


    int m = 5; //margin
    QImage preview = l.second.scaled(QSize(rect.width()-2*m,rect.height()-20));
    QRect frame(QPoint(rect.left()+m,rect.top()+m),preview.size());
    painter->fillRect(frame, QPixmap(":/icons/background.png"));

    if (option.state & QStyle::State_Selected) {
        QImage select(rect.size(), QImage::Format_ARGB32);
        select.fill(qRgba(12,79,166,64));
        painter->drawImage(rect.topLeft(),select);
    }

     painter->drawImage(QPoint(rect.left()+m,rect.top()+m),preview);


     QRect text(frame.bottomLeft(),rect.bottomRight());
     painter->drawRect(frame);

     painter->drawText(text,l.first);




}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(156,130);
}
