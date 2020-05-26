#ifndef COUNTMODEL_H
#define COUNTMODEL_H

#include "counter.h"
#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QMimeData>
#include <QDebug>

class CountModel : public QAbstractListModel
{
    Q_OBJECT
    QVector<Counter> _data;
public:
    explicit CountModel(QObject *parent = nullptr);
    explicit CountModel(QStringList& l, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

   inline Qt::DropActions supportedDropActions() const override{
       return Qt::CopyAction | Qt::MoveAction;
   }
   inline Qt::DropActions supportedDragActions() const override{
       return Qt::CopyAction | Qt::MoveAction;
   }
   inline Qt::ItemFlags flags(const QModelIndex& index) const override{
       if (index.isValid())
       {
           return  QAbstractListModel::flags(index)| Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;
       }

       return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled| Qt::ItemIsDropEnabled|Qt::ItemIsEnabled;
   }
   inline bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override {
       beginRemoveRows(parent,row,row+count -1 );
       _data.remove(row,count);
       endRemoveRows();
       return true;
   }
    inline bool insertRows(int row, int count, const QModelIndex& parent) override
    {
        beginInsertRows(QModelIndex(), row, row);
        endInsertRows();
        return true;
    }

    bool setData(const QModelIndex& idx, const QVariant& value, int role) override;


    inline QStringList mimeTypes() const override {
        QStringList types;
          types << "application/vnd.text.list";
          return types;
    }
    inline QMimeData *mimeData(const QModelIndexList &indexes) const
    {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        foreach (QModelIndex index, indexes) {
            if (index.isValid()) {;
                stream << index.data().value<Counter>();
            }
        }

        mimeData->setData("application/vnd.text.list", encodedData);
        return mimeData;
    }
    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

};




#endif // COUNTMODEL_H
