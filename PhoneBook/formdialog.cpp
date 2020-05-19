#include "formdialog.h"
#include "ui_formdialog.h"

#include <QLabel>
#include <QMessageBox>
#include <QStringListModel>
#include <QTableView>
#include <QDebug>
QRegExp  FormDialog::phoneReg("^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\\s\\./0-9]*$");

FormDialog::FormDialog(QAbstractItemModel* cmodel,const QModelIndex& ind, Contact& c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormDialog),_contactmodel(cmodel),_ind(ind),_contact(c)
{

    ui->setupUi(this);

    nameLabel  = ui->contactName;
    numsList = ui->numsList;

    nameLabel->setText(_contact.name());
    ui->nameInput->setText(_contact.name());


    _numbermodel = new NumbersModel(_contact.nums(),this);

    numsList->setModel(_numbermodel);

}

FormDialog::~FormDialog()
{
    delete ui;
}



void FormDialog::on_removeNum_clicked()
{
        QModelIndex index = ui->numsList->currentIndex();

        _numbermodel->removeRow(index.row());
}


void FormDialog::on_addNum_clicked()
{
    QString phone = ui->numberInput->text();
    if(!phoneReg.exactMatch(phone)){
        QMessageBox::warning(
            this,
            tr("Contact form"),
            tr("Incorrect phone.") );
        return;

    }
    _numbermodel->insertItem(phone);
}

void FormDialog::on_cancel_clicked()
{
    close();
}

void FormDialog::on_commit_clicked()
{
    if(nameLabel->text().isEmpty()){
        QMessageBox::warning(
            this,
            tr("Contact form"),
            tr("Empty name.") );
        return;
    }


    _contact.setName(nameLabel->text());
    _contact.setNums(_numbermodel->numbers());
    close();
}

void FormDialog::on_deleteContact_clicked()
{
    _contactmodel->removeRow(_ind.row());
    close();
}

void FormDialog::on_nameInput_textChanged(const QString &name)
{
    nameLabel->setText(name);

}


