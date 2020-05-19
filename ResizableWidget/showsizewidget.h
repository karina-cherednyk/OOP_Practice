#ifndef SHOWSIZEWIDGET_H
#define SHOWSIZEWIDGET_H

#include <QWidget>

namespace Ui {
class ShowSizeWidget;
}

class ShowSizeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowSizeWidget(QWidget *parent = nullptr);
    ~ShowSizeWidget();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private:
    Ui::ShowSizeWidget *ui;
};

#endif // SHOWSIZEWIDGET_H
