#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QString>

class Layer {
    QString _layerName;
    QImage _content;
    bool _visible;
public:
    inline Layer(const QString& s, const QImage& im, bool v):_layerName(s),_content(im),_visible(v){}
    QString& name() { return _layerName; }
    QImage& content() { return  _content; }
    bool& visible() { return _visible;}
    const QString& name() const { return _layerName; }
    const QImage& content() const { return  _content; }
    const bool& visible() const { return _visible;}
};
#endif // LAYER_H
