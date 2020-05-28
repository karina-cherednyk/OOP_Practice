#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contactlistmodel.h"
#include "formdialog.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
 ContactListModel* _model;
 QSortFilterProxyModel* _proxy;
 FormDialog* _form;
 QString _fileName;
public:
    MainWindow(const QString& fileName = ":/contacts/contacts.txt", QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openNewWindow(const QModelIndex &) ;


    void on_findPhoneInput_textChanged(const QString &arg1);

    void on_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent* e) override;
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
