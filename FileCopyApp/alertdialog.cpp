#include "alertdialog.h"
#include "ui_alertdialog.h"

AlertDialog::AlertDialog(bool fromFileNotFound, bool toFileNotFound, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertDialog)
{
    ui->setupUi(this);
    QString alertMessage = QString(fromFileNotFound ? "File to copy from not specified\n": "") + QString(toFileNotFound ? "File to copy to is not specified" : "");

    ui->alertLabel->setText(alertMessage);
}


AlertDialog::~AlertDialog()
{
    delete ui;
}

void AlertDialog::on_button_clicked()
{
    close();
}
