#include "alertdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fromLabel = ui->fromLabel;
    toLabel = ui->toLabel;
    fromText = ui->fromText;
    toText = ui->toText;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_copyFromButton_clicked()
{
    from =  QFileDialog::getOpenFileName(
            this,
            "Open Document",
            QDir::currentPath(),
            "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    QFile file(from);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         from  = "";
         return;
     }
    fromLabel->setText(QString("Selected file: %0").arg(from));

    QTextStream in(&file);
    writeToBrowser(in);
    file.close();

}

void MainWindow::on_copyToButton_clicked()
{
    to =  QFileDialog::getOpenFileName(
            this,
            "Open Document",
            QDir::currentPath(),
            "Text files (*.txt) ;; Document files (*.doc *.rtf);; ");

    toLabel->setText(QString("Selected file: %0").arg(from));


}

void MainWindow::on_commitButton_clicked()
{

    bool fromFileNotSpecified = from.isEmpty();
    bool toFileNotSpecified = to.isEmpty();

    if(fromFileNotSpecified || toFileNotSpecified){
        dialog = new AlertDialog(fromFileNotSpecified, toFileNotSpecified, this);
        dialog->show();
        return;
    }
     QFile file(to);

     if(file.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QTextStream out(&file);
         out << toText->toPlainText();

         QMessageBox::information(
               this,
               tr("Success"),
               tr("Succesfully written to ") + file.fileName() );
     }
     else
         QMessageBox::warning(
               this,
               tr("Warning"),
               file.errorString() );


     file.close();
}

void MainWindow::writeToBrowser(QTextStream &in)
{
    QString res;
    while (!in.atEnd()) {

        res += in.read(1);
    }

    fromText->setText(res);
    toText->setText(res.replace('\t',' ').remove('\n'));
}
