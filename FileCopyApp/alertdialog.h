#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>

namespace Ui {
class AlertDialog;
}

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(bool fromFileNotFound, bool toFileNotFound, QWidget *parent = nullptr);
    explicit AlertDialog(const QString msg, QWidget *parent = nullptr);
    ~AlertDialog();

private slots:
    void on_button_clicked();

private:
    Ui::AlertDialog *ui;
};

#endif // ALERTDIALOG_H
