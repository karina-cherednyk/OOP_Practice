#include "contactlistmodel.h"

#include <QFile>
#include <QLinkedList>
#include <QTextStream>

ContactListModel::ContactListModel(QObject *parent, QString fileName)
    : QAbstractListModel(parent)
{


    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QStringList line = in.readLine().split(',');
          QString name = line.first();
          line.removeFirst();
          _contacts.append(Contact(name, line));

       }
       inputFile.close();
    }
}

QVariant ContactListModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    return QVariant();
}

int ContactListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _contacts.size();
}

QVariant ContactListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if( role == Qt::DisplayRole){

        return _contacts.at(index.row()).name();
    }

    return QVariant();
}

Contact& ContactListModel::contact(const QModelIndex &index)
{
    return _contacts[index.row()];

}

const QVector<Contact> &ContactListModel::contacts() const
{
    return _contacts;

}

const QModelIndexList ContactListModel::contactsWithNumber(const QString &num) const
{

   QModelIndexList list;
   if(num.isEmpty()) return list;
   for(int i=0; i<_contacts.size(); ++i){
       if(_contacts.at(i).hasMatchingPhone(num)) list.append(index(i));
   }
   return list;
}

void ContactListModel::addContact(const Contact &c)
{
    beginInsertRows(QModelIndex(),rowCount(), rowCount());
    _contacts.append(c);
    endInsertRows();
}

bool ContactListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(),row,count);

    _contacts.remove(row,count);
    endRemoveRows();
    return true;
}
