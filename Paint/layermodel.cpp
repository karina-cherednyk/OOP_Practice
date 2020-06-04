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

    const Layer& l = _layers->at(index.row());
    QPair<QString,QImage> pair(l.name(),l.content());
    return QVariant::fromValue<QPair<QString,QImage>>(pair);
}

/**
 * called when new layer name is set in delegate qLineEdit
 */
bool LayerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    (*_layers)[index.row()].name() = value.toString();
    return true;
}

/**
 * is called one time inside Canvas class to pass pointer to its own layers list
 */
void LayerModel::setLayersModel(QList<Layer > *layers)
{
    _layers = layers;
}



