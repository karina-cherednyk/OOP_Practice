#ifndef COUNTMODEL_H
#define COUNTMODEL_H

#include <QAbstractListModel>

class CountModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CountModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // COUNTMODEL_H
