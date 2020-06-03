#ifndef ColorPicker_H
#define ColorPicker_H

#include <QWidget>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class ColorPicker; }
QT_END_NAMESPACE

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    ColorPicker(QWidget *parent = nullptr);
    ~ColorPicker();

public slots:
    void changeSample(QColor color);
    void changeGradient(QColor color);

public:
    int curr_red = 0;
    int curr_green = 0;
    int curr_blue = 0;
    int curr_trans = 255;

private slots:
    void on_horizontalSlider_valueChanged(int position);

private:
    QWidget* sample;
    QPalette pal_sample;
    Ui::ColorPicker *ui;
};
#endif // ColorPicker_H
