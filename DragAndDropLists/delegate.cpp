#include "counter.h"
#include "delegate.h"
#include <QDebug>

ColorDelegate::ColorDelegate(Qt::GlobalColor color,QObject *parent) :  QItemDelegate(parent),_color(color) {}
void ColorDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

    Counter d = index.data().value<Counter>();

    QString text = d.name();
    if(d.count()) text+="(" +QString::number(d.count()) + ")";

    if (option.state & QStyle::State_Selected) {
        painter->setPen(Qt::white);
        painter->setBrush(_color);
        painter->fillRect(option.rect, _color);
    }
    else
    {
        painter->setPen(QPen(option.palette.foreground(), 0));
        painter->setBrush(qvariant_cast<QBrush>(index.data(Qt::ForegroundRole)));
    }
    painter->drawText( option.rect, text );
}

QSize ColorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{

    return  QSize(30,35);
}
