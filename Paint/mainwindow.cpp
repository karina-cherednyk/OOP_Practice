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
#include "filterform.h"
#include "layerdelegate.h"
#include "layermodel.h"
#include <QModelIndex>
#include <QToolButton>
#include <filters/brightnessfilter.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    _canvas(this) , ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->container->setLayout(new QHBoxLayout);
    ui->container->layout()->setMargin(0);
    ui->container->layout()->addWidget(&_canvas);

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
          QString text = QString(format).toUpper();
          QAction *action = new QAction(text, this);
          action->setData(format);
          connect(action, SIGNAL(triggered()), this, SLOT(save()));

          ui->menu_Save_As->addAction(action);
      }

    //main window only events
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()) );
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionPen_color,SIGNAL(triggered()), SLOT(invokePenColorDialog()));
    connect(ui->actionPen_width,SIGNAL(triggered()), SLOT(invokePenWidthDialog()));
    connect(ui->actionClear_screen, SIGNAL(triggered()), &_canvas,SLOT(clearImage()));

    //canvas redo/ undo actions
    connect(ui->actionRedo,SIGNAL(triggered()), &_canvas, SLOT(redo()));
    connect(ui->actionUndo,SIGNAL(triggered()), &_canvas, SLOT(undo()));
    connect(&_canvas, SIGNAL(undoSignal(bool)), ui->actionUndo, SLOT(setEnabled(bool))); //when current save is first
    connect(&_canvas, SIGNAL(redoSignal(bool)), ui->actionRedo, SLOT(setEnabled(bool))); //when current save is last

    //canvas sets tool to paste
    connect(ui->actionCut, SIGNAL(triggered()), &_canvas, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), &_canvas, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), &_canvas, SLOT(paste()));
    connect(&_canvas, SIGNAL(pasteSignal()),  SLOT(enablePaste())); //is called after area was selected with Cut or Copy
    //disable when no area selected
    connect(&_canvas, SIGNAL(enableCutCopy(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(&_canvas, SIGNAL(enableCutCopy(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(&_canvas, SIGNAL(enableCutCopy(bool)), ui->actionPaste, SLOT(setEnabled(bool)));



    // add/ remove layout
    QWidget* rightDockContainer = ui->rightDockWidget->widget();
    (rightDockContainer->findChild<QToolButton*>("addLayerButton"))->setDefaultAction(ui->actionAddLayer);
    (rightDockContainer->findChild<QToolButton*>("removeLayerButton"))->setDefaultAction(ui->actionRemoveLayer);

    layerList = rightDockContainer->findChild<QListView*>("layerList");
    layerList->setItemDelegate(new LayerDelegate(this));
    layerList->setModel(_canvas.getModel());
    layerList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //change current Canvas::_image to draw on
    connect(layerList, SIGNAL(pressed(const QModelIndex&)),  &_canvas, SLOT(setCurrentLayer(const QModelIndex&)));
    layerList->setCurrentIndex(_canvas.getModel()->index(0));

    //when layer is added deleted, next selection is determined insede Canvas class
    connect(&_canvas, SIGNAL(setSelected(const QModelIndex&)), layerList, SLOT(setCurrentIndex(const QModelIndex&)));


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

/**
 * open save dialog when canvas was modified after last save
 */
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

/**
 * save canvas current _image, after that Canvas::_modified == false
 */
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
    if(requestSaving()){ //save image before open next one
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

void MainWindow::on_actionColorpicker_triggered()
{
    _canvas.setTool(Canvas::ColorPicker);
}

void MainWindow::on_actionRemoveLayer_triggered()
{
    const QModelIndex& selected = layerList->selectionModel()->currentIndex();
    _canvas.removeLayer(selected);
}

void MainWindow::on_actionAddLayer_triggered()
{
    const QModelIndex& selected = layerList->selectionModel()->currentIndex();
    _canvas.insertLayer(selected);
}

void MainWindow::on_actionBrightness_triggered()
{
    FilterForm* f = new FilterForm("Brightness filter",new BrightnessFilter, _canvas.getResImage(),this);

    f->show();
    connect(f, SIGNAL(addImage(const QString&, const QImage&)), &_canvas, SLOT(addImage(const QString&, const QImage&)));

}
