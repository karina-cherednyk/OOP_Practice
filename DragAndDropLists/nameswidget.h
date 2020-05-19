#ifndef NAMESWIDGET_H
#define NAMESWIDGET_H

#include <QWidget>

namespace Ui {
class NamesWidget;
}

class NamesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NamesWidget(QWidget *parent = nullptr);
    explicit NamesWidget(QString fileName,QWidget *parent = nullptr);
    ~NamesWidget();

private:
    Ui::NamesWidget *ui;
};

#endif // NAMESWIDGET_H
