#ifndef LAYERMODEL_H
#define LAYERMODEL_H
#include "Pair.h"

#include <QDebug>
#include <QAbstractListModel>
#include <QItemDelegate>


class LayerModel : public QAbstractListModel
{
    Q_OBJECT
    QList<Pair<QString,QImage>>* _layers;


public:


    explicit LayerModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    inline void setLayersModel(QList<Pair<QString,QImage>>* list){
        _layers = list;
    }
    inline bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override{
        beginInsertRows(parent, row, row+count-1);
        endInsertRows();
        return true;
    }
    inline bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override{
        beginRemoveRows(parent, row, row+count-1);
        endRemoveRows();
        return true;
    }

private:
};
class Delegate : public  QItemDelegate
{
    Q_OBJECT
public:
    Delegate( QObject* p=nullptr) ;

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // LAYERMODEL_H
