#ifndef LAYER_H
#define LAYER_H

#include <QImage>
#include <QString>


class Layer {
    QString _layerName;
    QImage* _content;
    bool _visible;
public:
    inline Layer(const QString& s, const QImage& im, bool v):_layerName(s),_content(new QImage(im)),_visible(v){}
    inline QString& name() { return _layerName; }
    inline  QImage& content() { return  *_content; }
    inline bool& visible() { return _visible;}
    inline const QString& name() const { return _layerName; }
    inline const QImage& content() const { return  *_content; }
    inline const bool& visible() const { return _visible;}
    const Layer& operator=(const Layer& l) = delete ;
};
#endif // LAYER_H
