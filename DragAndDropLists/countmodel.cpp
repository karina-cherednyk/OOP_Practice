#include "countmodel.h"
#include <QDebug>


CountModel::CountModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CountModel::CountModel(QStringList &l, QObject *parent)
{
    for(const QString& name: l) _data.append(Counter(name));
}

int CountModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.size();
}

QVariant CountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

   // qDebug() << _data[index.row()].name() ;

    return QVariant::fromValue<Counter>( _data[index.row()] );
}

bool CountModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    int r;

    //This role is used when double-clicking on an item
    if (idx.isValid() && role == Qt::EditRole) {
        return true;
    }

    //And this one when drag and dropping
    else if (role == Qt::DisplayRole)
    {
        r = idx.row();
        if(r==-1) r = _data.size();
        if(_data.size()<=r) _data.append(++value.value<Counter>());
        else _data.insert(r,++value.value<Counter>());

    }


    QModelIndex idx2 = index(r,0,QModelIndex());
    emit dataChanged(idx2,idx2);
    return true;
}

bool CountModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;


    int beginRow;

    if (row != -1)  beginRow = row;
    else if (parent.isValid()) beginRow = parent.row();
    else beginRow = rowCount(QModelIndex());



    //

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<Counter> newItems;
    int rows = 0;

    while (!stream.atEnd()) {
        Counter item;
        stream >> item;
        newItems << item;
        ++rows;
    }
    insertRows(beginRow, rows, QModelIndex());
    foreach (Counter c, newItems) {

        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, QVariant::fromValue<Counter>(c), Qt::DisplayRole);
        beginRow++;
    }

    return true;

}



