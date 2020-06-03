#include "canvas.h"
#include "ui_canvas.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QStack>
#include <QModelIndex>
#include <QAbstractListModel>
#include <QRandomGenerator>
#include <cassert>

Canvas::Canvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Canvas),_penWidth(1),_penColor(Qt::black),
    _modified(false),_curSave(0),_lastAvailableSave(0),
    _tool(Pen),_model(this),_c(1),_selection(this),_shape(this),_CWCursor(QPixmap(":/icons/cw.png")),_ACWCursor(QPixmap(":/icons/acw.png"))
{
    ui->setupUi(this);

    _layers.append(Pair<QString,QImage>("Layer 0",QImage(width(), height(), QImage::Format_ARGB32)));
    _layers[0].second().fill(Qt::transparent);
    _model.setLayersModel(&_layers);
    _model.insertRow(0);

    _image = &_layers[0].second();

    _saves.append(QPair<QImage*,QImage>(_image,*_image));


     setFocusPolicy(Qt::StrongFocus);


}


void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect drawingArea = event->rect();


    for(int i=0; i<_layers.size();++i){
        painter.drawImage(drawingArea, _layers[i].second(),drawingArea);
    }

    if(_tool == Rotate){
        painter.translate(_rotation.center());
        painter.rotate(_rotation.angle());
        QPoint origin = _rotation.getAfterTransformOriginPoint();
        painter.drawImage(origin, _selection.getTransformedImage());
        painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        painter.drawRect(QRect(origin, _rotation.getRotatingRect().size()));
    }
    else if(_tool == Rectangle || _tool == Ellipse){
        painter.setPen(QPen(getToolColor(),_penWidth));
        if(_tool == Rectangle) painter.drawRect(_shape.getWorkingRect());
        else  painter.drawEllipse(_shape.getWorkingRect());
    }
    else if(_selection.hasSelection()){
        if(_selection.isFinished())
            painter.setPen(QPen(Qt::black,2,Qt::DashDotLine));
        else  painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        if(!_selection.isNoneMode()) painter.drawImage(_selection.getWorkingRect(),_selection.getTransformedImage());
        painter.drawRect(_selection.getWorkingRect());
    }

}


bool Canvas::openImage(const QString &fileName)
{
      QImage loadedImage;

      if (!loadedImage.load(fileName))
          return false;

      QSize newSize = loadedImage.size().expandedTo(size());
      resizeCanvas(&loadedImage, newSize);
      *_image = loadedImage;
      _modified = false;
      update();
      return true;
}

QImage Canvas::getResImage()
{
    QImage res = QImage(_image->size(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter p(&res);
    QPoint o(0,0);
     for(int i=0; i<_layers.size();++i){
        p.drawImage(o,_layers[i].second());
     }
     return res;
}


bool Canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage savingImage = getResImage();
    resizeCanvas(&savingImage, size());

    if(savingImage.save(fileName, fileFormat)){

        _modified = false;
        return true;
    }
    return false;
}


void Canvas::setTool(const Canvas::Tool &t)
{
    //toggle selection
    if(_tool == Select && t == Select){
        _selection.removeSelection();
        update( _selection.getWorkingRect().adjusted(-3,-3,3,3));
    }
    //currents selected area may be modified, so save new image in area
    if(_selection.hasSelection()) {
        _selection.setImage(_image->copy(_selection.getWorkingRect()));
    }
    if(_tool == Rotate){
        _tool = t;
        update(_rotation.getUpdateRect());
    }

    _tool = t;


}



void Canvas::redo()
{

    if(_curSave<_lastAvailableSave){

       QPair<QImage*, QImage> pair = _saves[++_curSave];
       *pair.first = pair.second;
       update();
    }
    emit redoSignal(_curSave+1<=_lastAvailableSave);
    emit undoSignal(true);
}

void Canvas::undo()
{
    QPair<QImage*, QImage> pair = _saves[--_curSave];
     *pair.first = pair.second;

    update();

    emit undoSignal( _curSave);
    emit redoSignal(true);


}
void clearArea(QImage& image, const QRect& area){
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(area, Qt::transparent);
}

void Canvas::cut()
{

    if(_selection.hasSelection()){
        _selection.setImage(_image->copy(_selection.getWorkingRect()));
        clearArea(*_image, _selection.getWorkingRect());
        update(_selection.getUpdateRect());
        saveState();
        emit pasteSignal();
    }

}



void Canvas::setCurrentLayer(const QModelIndex(& ind))
{
    _image = &_layers[ind.row()].second();
}

void Canvas::insertLayer(const QModelIndex &ind)
{
    int pos = _layers.empty() ? 0 : ind.row()+1;
    _layers.insert(pos,
                       Pair<QString,QImage>(
                       QString("Layer %0").arg(_c++),
                       QImage(width(), height(), QImage::Format_ARGB32)));
    _model.insertRow(pos);

    _image = &_layers[pos].second();
    _image->fill(qRgba(255,255,255,0));
     emit setSelected(_model.index(pos));
     setEnabled(true);
}

void Canvas::removeLayer(const QModelIndex &ind)
{
    if(_layers.empty()) return;

    int r = ind.row();


    if(_layers.size()==1){
        _image = nullptr;
        setEnabled(false);
    }
    else if(r!=0) { _image = &_layers[r-1].second(); emit setSelected(_model.index(r-1)); }
    else { _image = &_layers[r+1].second(); emit setSelected(_model.index(r)); }
    _layers.removeAt(r);
    _model.dataChanged(ind,ind);

}




//TODO add 4 cursors for rotation;
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint& p = event->pos();
    if(_tool == Pen || _tool == Eraser ) drawLineTo(p);
    else if(_tool == ColorPicker) pickColor(p);
    else if(_tool == Spray) drawSpray(p);
    else if(_tool == Rotate) {
        _rotation.setEnd(p);
         double angle = _rotation.angle();
         if(angle<180) setCursor(_CWCursor);
         else setCursor(_ACWCursor);
        update(_rotation.getUpdateRect());
    }
    else if(_tool == Select ) {
        _selection.processMove(p);
        update(_selection.getUpdateRect());
    }
    else if(_tool == Rectangle || _tool == Ellipse) {
        _shape.processMove(p);
        update(_shape.getUpdateRect());
    }


}


void Canvas::pasteImage(QPoint pos)
{
    _selection.setSelectionRect(QRect(pos,_selection.getWorkingRect().size()));
    _tool = Select;
    _selection.enablePaste();
    update(_selection.getUpdateRect());
}
void Canvas::prepareForRotation(QPoint pos)
{
    const QRect& selected = _selection.getWorkingRect();
    _rotation.setRotatingRect(selected);
    _rotation.setCenter(selected.center());
    _rotation.setBegin(pos);
    clearArea(*_image, selected);
}

void Canvas::prepareForShapeChange(ShapeEvent &e, QPoint p)
{
    e.processPress(p);
    update(e.getUpdateRect());
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    const QPoint& p = event->pos();
    if(_tool == Pen || _tool == Eraser) _lastPoint = p;
    else if(_tool == ColorPicker) pickColor(p);
    else if(_tool == Spray) drawSpray(p);
    else if(_tool == Bucket) fill(p);
    else if(_tool == Rotate) prepareForRotation(p);
    else if(_tool == Paste) pasteImage(p);
    else if(_tool == Select )   {

        _selection.processPress(p);
        if(!_selection.isNoneMode()){
            clearArea(*_image, _selection.getBufferedRect());
        }
        update(_selection.getUpdateRect());
        emit enableCutCopy(true);
    }
    else if(_tool == Rectangle || _tool == Ellipse) prepareForShapeChange(_shape,p);

}


int getIndex(const QImage* im,  QList<Pair<QString,QImage>>&list);
void Canvas::mouseReleaseEvent(QMouseEvent *event) {

    if(_tool == Select){

       _selection.processRelease();
       update(_selection.getUpdateRect());

       if(!_selection.isNoneMode()){
       QPainter painter(_image);
       painter.drawImage(_selection.getWorkingRect(), _selection.getTransformedImage());
       }
       _selection.setImage(_image->copy(_selection.getWorkingRect()));


    }
    else if(_tool == Rectangle || _tool == Ellipse){
        _shape.processRelease();
        update(_shape.getUpdateRect());
        QPainter painter(_image);
        painter.setPen(QPen(getToolColor(),_penWidth));
        if(_tool == Rectangle) painter.drawRect(_shape.getWorkingRect());
        else painter.drawEllipse(_shape.getWorkingRect());
    }
    else if(_tool == Rotate){
        QPainter painter(_image);
        painter.translate(_rotation.center());
        painter.rotate(_rotation.angle());
        QPoint origin = _rotation.getAfterTransformOriginPoint();
        painter.drawImage(origin, _selection.getTransformedImage());
        setCursor(Qt::ArrowCursor);
    }

    if( _tool != ColorPicker) saveState();

}



void Canvas::saveState()
{
    ++_curSave;
    if(_curSave == _saves.size())
        _saves.append(QPair<QImage*,QImage>(_image,*_image));

    else {
        _saves[_curSave] = QPair<QImage*,QImage>(_image,*_image);
    }

    _lastAvailableSave = _curSave;

    emit undoSignal(true);
    emit redoSignal(false);

    _modified = true;

    int curInd = getIndex(_image,_layers); assert(curInd!=-1);
    const QModelIndex& changed = _model.index(curInd);
    _model.dataChanged(changed, changed);
}



void Canvas::resizeEvent(QResizeEvent *event)
{
    if (width() > _image->width() || height() > _image->height()) {
        int newWidth = qMax(width() + 128, _image->width());
        int newHeight = qMax(height() + 128, _image->height());
        resizeCanvas(_image, QSize(newWidth, newHeight));
        update();
    }
       QWidget::resizeEvent(event);
}

void Canvas::drawLineTo(const QPoint &nextPoint)
{

    QPainter painter(_image);
    if(_selection.hasSelection()) painter.setClipRect(_selection.getWorkingRect());
    if(_tool == Eraser) painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(getToolColor(),_penWidth));
    painter.drawLine(_lastPoint,nextPoint);

    int rad = (_penWidth / 2) + 2;

    update(QRect(_lastPoint, nextPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));

    _lastPoint = nextPoint;
}



void Canvas::resizeCanvas(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize) return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(Qt::transparent);
    QPainter painter(&newImage);

    painter.drawImage(QPoint(0,0), *_image);
    *_image = newImage;
}

void Canvas::fill(QPoint pos){

    QColor color = _image->pixelColor(pos);
    QStack<QPoint> stack;
    stack.push(pos);

    int minX(0), maxX(_image->width()-1), minY(0), maxY(_image->height()-1);

    if(_selection.hasSelection()){
        const QRect& boundaries = _selection.getWorkingRect();
        minX = qMax(minX,boundaries.left());
        maxX = qMin(maxX,boundaries.right()-1);
        minY = qMax(minY,boundaries.top());
        maxY = qMin(maxY,boundaries.bottom()-1);
    }

    while(!stack.empty()){
        QPoint p = stack.pop();
        _image->setPixelColor(p,getToolColor());
        if(p.x() > minX && _image->pixelColor(p.x()-1,p.y()) == color) stack.push(QPoint(p.x()-1,p.y()));
        if(p.y() > minY && _image->pixelColor(p.x(),p.y()-1) == color) stack.push(QPoint(p.x(),p.y()-1));
        if(p.x() < maxX && _image->pixelColor(p.x()+1,p.y()) == color) stack.push(QPoint(p.x()+1,p.y()));
        if(p.y() < maxY && _image->pixelColor(p.x(),p.y()+1) == color) stack.push(QPoint(p.x(),p.y()+1));
    }
    update();

}


void Canvas::drawSpray(QPoint pos)
{
    QPainter painter(_image);
    if(_selection.hasSelection()) painter.setClipRect(_selection.getWorkingRect());

    painter.setPen(QPen(_penColor,_penWidth/4));
    int num = qrand() % 30;

    int x = pos.x(), y = pos.y();
    for(int i=0; i<num; ++i){
        int dx = QRandomGenerator::global()->bounded(-_penWidth, _penWidth *2);
        int dy = QRandomGenerator::global()->bounded(-_penWidth, _penWidth *2);
        painter.drawPoint(x+dx,y+dy);
    }
    update(QRect(-2*penWidth()+pos.x(),-2*penWidth()+pos.y(),4*_penWidth, 4*_penWidth));
}

QColor Canvas::getToolColor()
{
    if(_tool == Eraser) return Qt::transparent;
    return _penColor;
}


void Canvas::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_T) return;
    if(_tool == Rotate)  setTool(Select);
    else if( _selection.hasSelection()) setTool(Rotate);

}

LayerModel *Canvas::getModel()
{
    return &_model;
}

void Canvas::pickColor(QPoint pos){
    _penColor = _image->pixelColor(pos);
}
Canvas::~Canvas()
{
    delete ui;
}

QColor Canvas::penColor() const
{
    return _penColor;
}

int Canvas::penWidth() const
{
    return _penWidth;
}

bool Canvas::isModified() const
{
    return _modified;
}
void Canvas::setPenColor(const QColor &color)
{
    _penColor = color;
}

void Canvas::setPenWidth(int width)
{
    _penWidth = width;
}
void Canvas::copy()
{
     if(_selection.hasSelection()){
         _selection.setImage(_image->copy(_selection.getWorkingRect()));
         emit pasteSignal();
     }
}

void Canvas::paste()
{
    setTool(Paste);
}

void Canvas::clearImage()
{
    _image->fill(Qt::transparent);
    _modified = true;
    saveState();
    update();
}
int getIndex(const QImage* im,  QList<Pair<QString,QImage>>&list){
   for(int i=0; i<list.size(); ++i)
       if(&(list[i].second()) == im) return i;
   return -1;
}
