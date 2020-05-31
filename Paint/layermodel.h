#ifndef LAYERMODEL_H
#define LAYERMODEL_H
#include "Pair.h"

#include <QDebug>
#include <QAbstractListModel>
#include <QItemDelegate>
#include <QStyledItemDelegate>


class LayerModel : public QAbstractListModel
{
    Q_OBJECT
    QList<Pair<QString,QImage>>* _layers;


public:


    explicit LayerModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole) override;


    inline Qt::ItemFlags flags(const QModelIndex &index) const override {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }


    void setLayersModel(QList<Pair<QString,QImage>>* layers);

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




};

#endif // LAYERMODEL_H
