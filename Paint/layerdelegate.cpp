#include "layerdelegate.h"
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QVBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QCheckBox>
#include <QVariant>

LayerDelegate::LayerDelegate(QWidget *parent):QStyledItemDelegate(parent){

}

/**
 * editor data - layer name
 */
void LayerDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QPair<QString,QPair<bool,QImage>> data = index.model()->data(index, Qt::EditRole).value<QPair<QString,QPair<bool,QImage>>>();
    QLineEdit* label = editor->findChild<QLineEdit*>();
    QCheckBox* box = editor->findChild<QCheckBox*>();
    label->setText(data.first);
    box->setChecked(data.second.first);

}

/**
 * renames selected layer with qLineEdit editor's text
 */
void LayerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
     QLineEdit* label = editor->findChild<QLineEdit*>();
     QCheckBox* box =  editor->findChild<QCheckBox*>();
     QString val = label->text();
     bool b = box->isChecked();
     if(!val.isEmpty()) {
         QPair<QString, bool> res(val,b);
         model->setData(index, QVariant::fromValue(res), Qt::EditRole);
     }

}

/**
 * paints qimage preview of layer content, name of layer
 */
void LayerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QString,QPair<bool,QImage>> l = index.data().value<QPair<QString,QPair<bool,QImage>>>();
    const QImage& im =  l.second.second;
    bool visible = l.second.first;
    QRect rect = option.rect;


    int m = 5; //margin
    QImage preview = im.scaled(QSize(rect.width()-2*m,rect.height()-25));
    QRect frame(QPoint(rect.left()+m,rect.top()+m),preview.size());

    painter->fillRect(frame, QPixmap(":/icons/background.png"));

    if (option.state & QStyle::State_Selected) {
        QImage select(rect.size(), QImage::Format_ARGB32);
        select.fill(qRgba(12,79,166,64));
        painter->drawImage(rect.topLeft(),select);
    }

     painter->drawImage(QPoint(rect.left()+m,rect.top()+m),preview);

     QPoint tL(rect.left()+10,rect.bottom()-20);
     QRect text(tL,rect.bottomRight());
     painter->drawRect(frame);

     painter->drawText(text,l.first);

    if(!visible) {
        QImage select(rect.size(), QImage::Format_ARGB32);
        select.fill(qRgba(0,0,0,64));
        painter->drawImage(rect.topLeft(),select);
    }
}


void LayerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    QPoint tL(rect.left(),rect.bottom()-20);
    QRect bounds(tL,rect.bottomRight());
    editor->setGeometry(bounds);

    QLineEdit* label = editor->findChild<QLineEdit*>();
    QCheckBox* box =  editor->findChild<QCheckBox*>();

    QRect boxRect = QRect(label->width()+10, 0, 20 , 20);
    box->setGeometry(boxRect);
}

QSize LayerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(),120);
}

QWidget *LayerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget* editor = new QFrame(parent);
    QLineEdit* lineEditor = new QLineEdit(editor);
    lineEditor->setStyleSheet("background: white");
    QCheckBox* box = new QCheckBox(editor);
    return editor;
}


