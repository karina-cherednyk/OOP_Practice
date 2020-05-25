#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"

#include <QGraphicsView>
#include <QMainWindow>
#include<QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Canvas _canvas;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui;
    bool requestSaving();

private slots:
    bool save(const QByteArray &fileFormat = "png");
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
};
#endif // MAINWINDOW_H
