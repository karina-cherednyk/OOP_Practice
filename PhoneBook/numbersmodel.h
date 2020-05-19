#ifndef MYSTRINGLISTMODEL_H
#define MYSTRINGLISTMODEL_H

#include <QAbstractListModel>

class NumbersModel : public QAbstractListModel
{
    Q_OBJECT
QList<QString> _numbers;
public:
    explicit NumbersModel(const QSet<QString>& nums, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertItem(const QString& num);

    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    const QList<QString> numbers() const;
private:
};

#endif // MYSTRINGLISTMODEL_H
