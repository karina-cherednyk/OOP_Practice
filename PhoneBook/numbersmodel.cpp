#include "numbersmodel.h"

#include <QSet>

NumbersModel::NumbersModel(const QSet<QString>& nums, QObject *parent)
    : QAbstractListModel(parent), _numbers(nums.toList())
{
}

QVariant NumbersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int NumbersModel::rowCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return 0;

    return _numbers.size();
}

QVariant NumbersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if(role == Qt::DisplayRole) return _numbers.at(index.row());
    return QVariant();
}

bool NumbersModel::insertItem(const QString &num)
{
    int row = _numbers.size();
    beginInsertRows(QModelIndex(), row, row);

    _numbers << num;

    endInsertRows();

   return true;
}
bool NumbersModel::removeRow(int row, const QModelIndex &parent){

    beginRemoveRows(QModelIndex(), row, row);
    _numbers.removeAt(row);
    endRemoveRows();
    return true;
}

const QList<QString> NumbersModel::numbers() const
{
    return _numbers;
}
