#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"

#include <QGraphicsView>
#include <QMainWindow>
#include<QDebug>
#include <QStackedLayout>
#include <QListView>

#include <filters/afilter.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
typedef Canvas::Tool Tool ;
class MainWindow : public QMainWindow
{
    Q_OBJECT


    QAction* _currTool;
    QMap<Tool, QIcon> _toolIcons;
    QWidget* _container;
     Canvas _canvas;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
   void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::MainWindow *ui;
    QListView* _layerList;
    bool requestSaving();
    void showFilterForm(AFilter*  f);
    void addToolmages();

private slots:
    bool save(const QByteArray &fileFormat);
    bool save();
    void open();
    void invokePenColorDialog();
    void invokePenWidthDialog();
    void on_actionBrush_triggered();
    void on_actionBucket_triggered();
    void on_actionSelect_triggered();
    void enablePaste();
    void on_actionRectangle_triggered();
    void on_actionEllipse_triggered();
    void on_actionEraser_triggered();
    void on_actionSpray_triggered();
    void on_actionColorpicker_triggered();


    void on_actionRemoveLayer_triggered();
    void on_actionAddLayer_triggered();
    void on_actionBrightness_triggered();

    void on_actionBlack_and_white_triggered();
    void on_actionContrast_triggered();
    void on_actionGray_triggered();
    void on_actionNegative_triggered();
    void on_actionNoise_triggered();
    void on_actionPerona_Malik_Blur_triggered();
    void on_actionRGBA_triggered();
    void on_actionSepia_triggered();
    void setCurrentTool(Tool t);
    void on_actionTriangle_triggered();
    void on_actionUpLayer_triggered();
    void on_actionDownLayer_triggered();
    void on_actionSelect_All_triggered();
    void on_actionNew_triggered();
};
#endif // MAINWINDOW_H
