#ifndef COLORSPECTRUM_H
#define COLORSPECTRUM_H
#include <QWidget>
#include <QObject>
#include <QPainter>
namespace Ui {
class ColorSpectrum;
}

class ColorSpectrum : public QWidget
{
    Q_OBJECT

public:
    inline ColorSpectrum(QWidget *parent = 0):QWidget(parent) {
        setAttribute(Qt::WA_StyledBackground, true);
    }
    ~ColorSpectrum() {}
    void mousePressEvent(QMouseEvent * event);
signals:
    void colorSelected(QColor color);
};

#endif // COLORSPECTRUM_H
