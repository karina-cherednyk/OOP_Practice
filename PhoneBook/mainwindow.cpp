#include "contactlistmodel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLinkedList"

#include <QSetIterator>
#include <QDebug>
#include <QFile>
#include <QSortFilterProxyModel>
#include <QFileInfo>

MainWindow::MainWindow(const QString& fileName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _fileName(fileName)
{
    ui->setupUi(this);
    QListView* namesList = ui->contactNamesList;
    QLineEdit *filter = ui->filterInput;



    _model = new ContactListModel(this, _fileName);
    _proxy  = new QSortFilterProxyModel(this);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(_model);
    namesList->setModel(_proxy);
    _proxy->sort(0);


    connect(filter, SIGNAL(textChanged(QString)), _proxy, SLOT(setFilterRegExp(QString))  );

    connect(namesList,  SIGNAL(clicked(const QModelIndex &)), this, SLOT(openNewWindow(const QModelIndex &)) );


}

void MainWindow::openNewWindow(const QModelIndex& index){

   _form = new FormDialog(
               _proxy, index,
               _model->contact(_proxy->mapToSource(index)),
               this
               );

   connect(_form, SIGNAL(destroyed()), SLOT(show()));
   connect(_form, SIGNAL(finished(int)), SLOT(show()));



   _form->show();
   hide();


}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QFile outFile("../PhoneBook/contacts.txt");
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << QFileInfo(outFile).absoluteFilePath() ;
        QTextStream stream(&outFile);
        QVectorIterator<Contact> it(_model->contacts());


        while(it.hasNext()){
           const Contact& c = it.next();
           stream << c.name()  << ',';

           QSetIterator<QString> nums = c.nums();
           while(nums.hasNext()){
               stream <<nums.next();
               if(nums.hasNext()) stream << ',';
           }
           stream << endl;
        }
    } else qDebug() << outFile.errorString();
    outFile.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_findPhoneInput_textChanged(const QString &phone)
{
    QModelIndex ind = _model->contactWithNumber(phone);
    ui->contactNamesList->setCurrentIndex(_proxy->mapFromSource(ind));
}
