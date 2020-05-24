#include "canvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QAction>
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    _canvas(this) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(&_canvas);


    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
          QString text = QString(format).toUpper();
          QAction *action = new QAction(text, this);
          action->setData(format);
          connect(action, SIGNAL(triggered()), this, SLOT(save()));

          ui->menu_Save_As->addAction(action);
      }

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()) );
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionPen_color,SIGNAL(triggered()), SLOT(invokePenColorDialog()));
    connect(ui->actionPen_width,SIGNAL(triggered()), SLOT(invokePenWidthDialog()));
    connect(ui->actionClear_screen, SIGNAL(triggered()), &_canvas,SLOT(clearImage()));

    connect(ui->actionRedo,SIGNAL(triggered()), &_canvas, SLOT(redo()));
    connect(ui->actionUndo,SIGNAL(triggered()), &_canvas, SLOT(undo()));


    connect(ui->actionCut, SIGNAL(triggered()), &_canvas, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), &_canvas, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), &_canvas, SLOT(paste()));


    connect(&_canvas, SIGNAL(undoSignal(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(&_canvas, SIGNAL(redoSignal(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

    connect(&_canvas, SIGNAL(pasteSignal()),  SLOT(enablePaste()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(requestSaving()) event->accept();
    else event->ignore();
}

bool MainWindow::requestSaving()
{
    if(_canvas.isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this,
                                   "Saving...",
                                   "Do you want to save progress?",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save) return save("png");
        else if(ret == QMessageBox::Cancel) return false;

    }
    return true;
}

bool MainWindow::save(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

   QString fileName = QFileDialog::getSaveFileName(this, "Save As",
                              initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                              .arg(QString::fromLatin1(fileFormat.toUpper()))
                              .arg(QString::fromLatin1(fileFormat)));

   if (fileName.isEmpty())
       return false;

   return _canvas.saveImage(fileName, fileFormat.constData());

}

void MainWindow::open()
{
    if(requestSaving()){
        QString fileName = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath());
        if(!fileName.isEmpty()) _canvas.openImage(fileName);
    }
}

void MainWindow::invokePenColorDialog()
{
    _canvas.setPenColor(QColorDialog::getColor(_canvas.penColor()));
}

void MainWindow::invokePenWidthDialog()
{
   bool ok;
   int newWidth = QInputDialog::getInt(this, "Width",
                                        "Select pen width:",
                                        _canvas.penWidth(),
                                        1, 50, 3, &ok);

   if(ok) _canvas.setPenWidth(newWidth);
}





void MainWindow::on_actionBrush_triggered()
{
    _canvas.setTool(Canvas::Pen);
}

void MainWindow::on_actionBucket_triggered()
{
    _canvas.setTool(Canvas::Bucket);
}

void MainWindow::on_actionSelect_triggered()
{
    _canvas.setTool(Canvas::Select);
}

void MainWindow::enablePaste()
{
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionRectangle_triggered()
{
    _canvas.setTool(Canvas::Rectangle);
}

void MainWindow::on_actionEllipse_triggered()
{
    _canvas.setTool(Canvas::Ellipse);
}

void MainWindow::on_actionEraser_triggered()
{
     _canvas.setTool(Canvas::Eraser);
}

void MainWindow::on_actionSpray_triggered()
{
    _canvas.setTool(Canvas::Spray);
}
