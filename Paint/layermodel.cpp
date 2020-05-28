#include "layermodel.h"

#include <QPainter>

LayerModel::LayerModel(QObject *parent)
    : QAbstractListModel(parent),_layers(nullptr)
{
}

int LayerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !_layers)
        return 0;
    return _layers->size();

}

/**
 * returns QPair<layer name, layer image> that corresponds to changeable Pair
 * TODO if you decide to add visible/hidden layer toggle button,
 * replace QPair qith custom class, add QOBJECT_METATYPE(className) to pass data in QVariant
 */
QVariant LayerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QPair<QString,QImage> pair(_layers->operator[](index.row()).operator QPair<QString,QImage>());
    return QVariant::fromValue<QPair<QString,QImage>>(pair);
}

/**
 * called when new layer name is set in delegate qLineEdit
 */
bool LayerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    (*_layers)[index.row()].first() = value.toString();
    return true;
}

/**
 * is called one time inside Canvas class to pass pointer to its own layers list
 */
void LayerModel::setLayersModel(QList<Pair<QString, QImage> > *layers)
{
    _layers = layers;
}



