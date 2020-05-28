#include "layerdelegate.h"

#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>

LayerDelegate::LayerDelegate(QWidget *parent):QStyledItemDelegate(parent){

}

void LayerDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QPair<QString,QImage> data = index.model()->data(index, Qt::EditRole).value<QPair<QString,QImage>>();

    QLineEdit* label = qobject_cast<QLineEdit*>(editor);
    label->setText(data.first);


}

void LayerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
     qDebug() << "setting model data";
     QLineEdit* label = qobject_cast<QLineEdit*>(editor);
     QString val = label->text();
     if(!val.isEmpty())
         model->setData(index, val, Qt::EditRole);
}

void LayerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    QPoint tL(rect.left(),rect.bottom()-12);
    QRect text(tL,rect.bottomRight());
    editor->setGeometry(text);
}

QSize LayerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(),120);
}

QWidget *LayerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setStyleSheet("background: white");
    return editor;
}

void LayerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

     QPoint tL(rect.left()+10,rect.bottom()-12);
     QRect text(tL,rect.bottomRight());
     painter->drawRect(frame);

     painter->drawText(text,l.first);




}
