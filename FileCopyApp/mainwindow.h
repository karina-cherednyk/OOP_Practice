#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "alertdialog.h"

#include <QLabel>
#include <QMainWindow>
#include <QTextBrowser>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString from;
    QString to;
    QLabel* fromLabel;
    QLabel* toLabel;
    QTextBrowser* fromText;
    QTextBrowser* toText;
    AlertDialog* dialog;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_copyFromButton_clicked();

    void on_copyToButton_clicked();

    void on_commitButton_clicked();

private:
    Ui::MainWindow *ui;
    void writeToBrowser(QTextStream& in);
};
#endif // MAINWINDOW_H
