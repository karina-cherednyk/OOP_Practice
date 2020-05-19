#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include "Contact.h"

#include <QAbstractListModel>

class ContactListModel : public QAbstractListModel
{
    Q_OBJECT
    QVector<Contact> _contacts;
public:
    explicit ContactListModel(QObject *parent = nullptr, QString fileName = "");

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Contact& contact(const QModelIndex &index);

    const QVector<Contact>& contacts() const;

    const QModelIndex& contactWithNumber(const QString& num) const;

protected:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
private:
};

#endif // CONTACTLISTMODEL_H
