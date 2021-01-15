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
#include "resizable_inner_widget.h"

Canvas::Canvas(QWidget *parent) :
    ResizableInnerWidget(parent),
    ui(new Ui::Canvas),
    _penWidth(5),_penColor(Qt::black),
    _modified(false),_curSave(-1),_lastAvailableSave(-1),
    _image(nullptr),_model(this),
    _selection(this),_shape(this),
    _tool(Brush),_c(0),
    _CWCursor(QPixmap(":/icons/cw.png")),_ACWCursor(QPixmap(":/icons/acw.png"))
{
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background-image: url(:/icons/background.png);border: 1px solid grey");
    ui->setupUi(this);
    connect(&_model, SIGNAL(changedIndex(const QModelIndex& )),SLOT(setCurrentLayer(const QModelIndex& )));
    _model.setLayersModel(&_layers);
    setEnabled(false);
    setFocusPolicy(Qt::StrongFocus);
}
void drawTriangle(QPainter& p, const QRect& bounds){
    QPoint topMiddle((bounds.left()+bounds.right())/2, bounds.top());
    p.drawLine(bounds.bottomLeft(), topMiddle);
    p.drawLine(bounds.bottomRight(), topMiddle);
    p.drawLine(bounds.bottomLeft(), bounds.bottomRight());
}
int getIndex(const QImage* im,  QList<Layer>&list){
   for(int i=0; i<list.size(); ++i)
       if(&(list[i].content()) == im) return i;
   return -1;
}
void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect drawingArea = event->rect();

    for(int i=0; i<_layers.size();++i){
       if(_layers[i].visible())  {
           resizeImage(&_layers[i].content(), size());
           painter.drawImage(drawingArea, _layers[i].content(),drawingArea);
           if(&_layers[i].content() == _image){
               if(_tool == Rotate){
                   painter.save();
                   painter.translate(_rotation.center());
                   painter.rotate(_rotation.angle());
                   QPoint origin = _rotation.getAfterTransformOriginPoint();
                   painter.drawImage(origin, _selection.getTransformedImage());
                   painter.restore();
               }
               else if(_tool == Line){
                   painter.setPen(QPen(_penColor, _penWidth));
                   painter.drawLine(_pressPoint, _lastPoint);
               }
               else if(isShapeTool() && _shape.hasSelection()){
                   painter.setPen(QPen(getToolColor(),_penWidth));
                   drawShape(painter);
               }
               else if(_selection.hasSelection()){
                   if(!_selection.isNoneMode()) painter.drawImage(_selection.getWorkingRect(),_selection.getTransformedImage());
               }
           }
       }
    }

    if(_tool == Rotate){
        painter.translate(_rotation.center());
        painter.rotate(_rotation.angle());
        QPoint origin = _rotation.getAfterTransformOriginPoint();
        painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        painter.drawRect(QRect(origin, _rotation.getRotatingRect().size()));
    }
    else if(_selection.hasSelection()){
        if(_selection.isFinished())
            painter.setPen(QPen(Qt::black,2,Qt::DashDotLine));
        else  painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        painter.drawRect(_selection.getWorkingRect());
    }

}


bool Canvas::openImage(const QString &fileName)
{
      QImage loadedImage;

      if (!loadedImage.load(fileName))
          return false;

      QImage preparedImage(loadedImage.size(), QImage::Format_ARGB32);
      QPainter p(&preparedImage);
      p.drawImage(QPoint(0,0),loadedImage);


      insertLayer(_model.index(_layers.size()-1));
      *_image = preparedImage;
      resize(qMax(_image->width(),width()), qMax(_image->height(), height()));
      _modified = false;
      update();
      _model.dataChanged(_layers.size() - 1);
      saveState();
      return true;
}

QImage Canvas::getResImage()
{
    QImage res = QImage(size(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter p(&res);
    QPoint o(0,0);
     for(int i=0; i<_layers.size();++i){

         if(_layers[i].visible()) p.drawImage(o,_layers[i].content());
     }
     return res;
}


bool Canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage savingImage = getResImage();
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

    emit toolSet(_tool);

}

void Canvas::selectAll()
{
     _selection.setSelectionRect(geometry());
     _selection.setImage(*_image);
     _selection.setAdjustMode();
     setTool(Select);
     update();
}

void Canvas::removeAll()
{
    if(_layers.isEmpty()) return;


    for(int i=0; i<_layers.size(); ++i) removeImageFromSaves(&_layers[i].content());
    _layers.clear();
    _saves.clear();

    _selection.removeSelection();
    update();
    _model.dataChanged(0, _layers.size() - 1);
    setEnabled(false);
    emit redoSignal(false);
    emit undoSignal(false);
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
   _model.dataChanged(0, _layers.size() - 1);
   _selection.removeSelection();
}

void Canvas::undo()
{
    QPair<QImage*, QImage> pair = _saves[--_curSave];
     *pair.first = pair.second;

    update();

    emit undoSignal( _curSave);
    emit redoSignal(true);
    _model.dataChanged(0, _layers.size() - 1);
    _selection.removeSelection();

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
        _selection.removeSelection();
        update(_selection.getUpdateRect());
        _model.dataChanged(getIndex(_image, _layers));
        saveState();
        emit pasteSignal();
    }

}



void Canvas::setCurrentLayer(const QModelIndex(& ind))
{
    _image = &_layers[ind.row()].content();
    bool enabled = _layers[ind.row()].visible();
    setEnabled(enabled);
    resizeImage(_image,size());
    update();
}

void Canvas::insertLayer(const QModelIndex &ind)
{
    int pos = _layers.empty() ? 0 : ind.row()+1;
    _layers.insert(pos,
                       Layer{
                       QString("Layer %0").arg(_c++),
                       QImage(width(), height(), QImage::Format_ARGB32), true });

    _model.insertRow(pos);
    _image = &_layers[pos].content();
    _image->fill(qRgba(255,255,255,0));
     emit setSelected(_model.index(pos));
     setEnabled(true);
     saveState();
}

void Canvas::removeLayer(const QModelIndex &ind)
{
    if(_layers.empty()) return;

    int r = ind.row();


    if(_layers.size()==1){
        _image = nullptr;
        setEnabled(false);
    }
    else if(r!=0) { _image = &_layers[r-1].content(); emit setSelected(_model.index(r-1)); }
    else { _image = &_layers[r+1].content(); emit setSelected(_model.index(r)); }
    removeImageFromSaves(&_layers[r].content());
    _layers.removeAt(r);
    _model.dataChanged(r);
    update();

}



void Canvas::moveLayer(const QModelIndex& ind, bool up){
     int i = ind.row();
     int to = up ? i-1 : i+1;
     if(to==-1 || to==_layers.size() || _layers.isEmpty()) return;
      _layers.swapItemsAt(i,to);
      const QModelIndex& toInd = _model.index(to);
      _model.dataChanged(to,i);
      emit setSelected(toInd);
      update();
}


void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    ResizableInnerWidget::mouseMoveEvent(event);
    if(_beingResized) return;

    const QPoint& p = event->pos();
    if(_tool == Brush || _tool == Eraser ) drawLineTo(p);
    else if(_tool == Line) {_lastPoint = p; update(); }
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
    else if(isShapeTool()) {
        _shape.processMove(p);
        update(_shape.getUpdateRect());
    }


}


void Canvas::pasteImage(QPoint pos)
{
    _selection.setSelectionRect(QRect(pos,_selection.getWorkingRect().size()));
    _tool = Select;
    _selection.setAdjustMode();
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


void Canvas::mousePressEvent(QMouseEvent *event)
{
    ResizableInnerWidget::mousePressEvent(event);
    if(_beingResized) return;

    const QPoint& p = event->pos();
    if(_tool == Brush || _tool == Eraser || _tool == Line) _pressPoint= _lastPoint = p;
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
    else if(isShapeTool()) {
        _shape.processPress(p);
        update(_shape.getUpdateRect());
    }

}


void Canvas::mouseReleaseEvent(QMouseEvent *event) {

    ResizableInnerWidget::mouseReleaseEvent(event);
    if(_beingResized) return;

    if(_tool == Select){

       _selection.processRelease();
       update(_selection.getUpdateRect());

       if(!_selection.isNoneMode()){
       QPainter painter(_image);
       painter.drawImage(_selection.getWorkingRect(), _selection.getTransformedImage());
       }
       _selection.setImage(_image->copy(_selection.getWorkingRect()));


    }
    else if(isShapeTool()){
        _shape.processRelease();

        update(_shape.getUpdateRect());
        QPainter painter(_image);
        painter.setPen(QPen(getToolColor(),_penWidth));
        drawShape(painter);
    }
    else if(_tool == Rotate){
        QPainter painter(_image);
        painter.translate(_rotation.center());
        painter.rotate(_rotation.angle());
        QPoint origin = _rotation.getAfterTransformOriginPoint();
        painter.drawImage(origin, _selection.getTransformedImage());
        setCursor(Qt::ArrowCursor);
    }
    else if(_tool == Line){
        QPainter painter(_image);
        painter.setPen(QPen(_penColor, _penWidth));
        painter.drawLine(_pressPoint, _lastPoint);
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
    _model.dataChanged(curInd);

}



void Canvas::resizeEvent(QResizeEvent *event)
{

       if(!_layers.isEmpty()){
           int maxH = qMax(height(), _image->height());
           int maxW = qMax(width(), _image->width());
           resizeImage(_image, QSize(maxW, maxH));
       }
       ResizableInnerWidget::resizeEvent(event);
}

void Canvas::drawLineTo(const QPoint &nextPoint)
{
    QPainter painter(_image);
    if(_selection.hasSelection()) painter.setClipRect(_selection.getWorkingRect());
    if(_tool == Eraser) painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(getToolColor(),_penWidth));
    painter.drawLine(_pressPoint,nextPoint);

    int rad = (_penWidth / 2) + 2;

    update(QRect(_pressPoint, nextPoint)
           .normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    _pressPoint = nextPoint;
}



void Canvas::resizeImage(QImage *image, const QSize &newSize)
{
    if(_image->size() == newSize) return;
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

void Canvas::addImage(const QString &name, const QImage &im)
{
    int pos = _layers.size();
    _layers.insert(pos,
                       {
                       QString("Layer %0 - %1").arg(_c++).arg(name),
                       im.scaled(_image->size(), Qt::IgnoreAspectRatio),  true});
    _model.insertRow(pos);
     emit setSelected(_model.index(pos));
     _image = &_layers[pos].content();
     _selection.removeSelection();
     setEnabled(true);
     saveState();
}

void Canvas::pickColor(QPoint pos){
    setPenColor(_image->pixelColor(pos));
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
    emit colorSet(color);
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

bool Canvas::isShapeTool()
{
    return _tool == Rectangle || _tool == Ellipse || _tool == Triangle;
}

void Canvas::drawShape(QPainter &painter)
{
    if(_tool == Rectangle) painter.drawRect(_shape.getWorkingRect());
    else  if(_tool == Ellipse) painter.drawEllipse(_shape.getWorkingRect());
    else if( _tool == Triangle) drawTriangle(painter, _shape.getWorkingRect());
}

void Canvas::removeImageFromSaves(QImage *im)
{
    QList<QPair<QImage*, QImage>>::iterator it = _saves.begin();
    while (it != _saves.end()) {
      if ((*it).first == im)
        it = _saves.erase(it);
      else
        ++it;
    }
    _curSave = _lastAvailableSave = _saves.size()-1;
    if(_saves.isEmpty()) emit undoSignal(false);
    emit redoSignal(false);
}
