#ifndef LAYERDELEGATE_H
#define LAYERDELEGATE_H

#include <QStyledItemDelegate>



class LayerDelegate: public QStyledItemDelegate
{
public:
    LayerDelegate(QWidget* parent);

protected:
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;


    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;


    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // LAYERDELEGATE_H
