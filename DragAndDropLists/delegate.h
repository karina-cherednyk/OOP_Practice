#ifndef DELEGATE_H
#define DELEGATE_H


#include <QItemDelegate>
#include <QStyledItemDelegate>

class ColorDelegate : public  QItemDelegate
{
    Q_OBJECT
    Qt::GlobalColor _color;
public:
    ColorDelegate(Qt::GlobalColor color, QObject* p=nullptr) ;

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};
#endif // DELEGATE_H
