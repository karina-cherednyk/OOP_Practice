#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include "Contact.h"
#include "contactlistmodel.h"
#include "numbersmodel.h"

#include <QDialog>
#include <QLabel>
#include <QListView>
#include <QStringListModel>

namespace Ui {
class FormDialog;
}

class FormDialog : public QDialog
{
    QListView *numsList ;
    NumbersModel* _numbermodel;
    QLabel* nameLabel ;
    Q_OBJECT
QAbstractItemModel* _contactmodel;
Contact& _contact;
const QModelIndex& _ind;
static QRegExp phoneReg;

public:
    explicit FormDialog(QAbstractItemModel* _numbermodel,const QModelIndex& ind,Contact& c, QWidget *parent = nullptr);
    ~FormDialog();

private slots:
    void on_removeNum_clicked();

    void on_addNum_clicked();

    void on_cancel_clicked();

    void on_commit_clicked();

    void on_deleteContact_clicked();

    void on_nameInput_textChanged(const QString &arg1);


private:
    Ui::FormDialog *ui;
};

#endif // FORMDIALOG_H
