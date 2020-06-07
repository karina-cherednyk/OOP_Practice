#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QString>


class Layer {
    QString _layerName;
    QImage* _content;
    bool _visible;
public:
    inline Layer(const QString& s, const QImage& im,  bool v=true):
        _layerName(s),_content(new QImage(im)),_visible(v){}
    Layer(const Layer& l):
      _content(new QImage(*l._content)),_visible(l._visible),_layerName(l._layerName)
    { }

    inline QString& name() { return _layerName; }
    inline  QImage& content() { return  *_content; }
    inline bool& visible() { return _visible;}
    inline const QString& name() const { return _layerName; }
    inline const QImage& content() const { return  *_content; }
    inline const bool& visible() const { return _visible;}
    const Layer& operator=(const Layer& l) = delete ;
    inline ~Layer() { delete _content; }
};
#endif // LAYER_H
