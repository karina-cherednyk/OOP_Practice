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
    _tool(Pen),_selectionFinished(false), _selectionMode(None),_model(this),_c(1)
{
    ui->setupUi(this);

    _layers.append(Pair<QString,QImage>("Layer 0",QImage(width(), height(), QImage::Format_ARGB32)));
    _layers[0].second().fill(Qt::transparent);
    _model.setLayersModel(&_layers);
    _model.insertRow(0);

    _image = &_layers[0].second();

    _saves.append(QPair<QImage*,QImage>(_image,*_image));
    _selectionRect = nullptr;
    _bufferedRect = nullptr;

     setFocusPolicy(Qt::StrongFocus);
}

//TODO _bufferedImage is never drawn with Rotate tool
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
        painter.drawImage(origin, *_bufferedImage); //TODO is never drawn
        painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        painter.drawRect(QRect(origin, _rotation.getRotatingRect().size()));
    }
    else if(hasSelectedArea()){
        if(_tool == Rectangle || _tool == Ellipse) painter.setPen(QPen(_penColor,_penWidth));
        else if(_selectionFinished) painter.setPen(QPen(Qt::blue,2,Qt::DashDotLine));
        else  painter.setPen(QPen(Qt::black,2,Qt::DashDotLine));
        if(_tool == Ellipse) painter.drawEllipse(*_selectionRect);
        else painter.drawRect(*_selectionRect);
    }
}


bool Canvas::openImage(const QString &fileName)
{
      QImage loadedImage;

      if (!loadedImage.load(fileName))
          return false;

      QSize newSize = loadedImage.size().expandedTo(size());
      resizeImage(&loadedImage, newSize);
      *_image = loadedImage;
      _modified = false;
      update();
      return true;
}

bool Canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage savingImage = *_image;
    resizeImage(&savingImage, size());

    if(savingImage.save(fileName, fileFormat)){

        _modified = false;
        return true;
    }
    return false;
}


void Canvas::setTool(const Canvas::Tool &t)
{
    if(_tool == Select && t == Select){
        _selectionRect = nullptr;
        update( (*_bufferedRect).adjusted(-3,-3,3,3));
    }
    else emit enableCutCopy(false);
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

void Canvas::cut()
{

    if(_selectionRect){

        QPainter painter(_image);
        painter.fillRect(*_selectionRect, Qt::white);
        update(*_selectionRect);
        saveState();
        emit pasteSignal();
        setTool(Paste);
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

bool Canvas::resizeSelectionArea(QPoint pos)
{
    if(_selectionMode == Move){
        int dx = _bufferedRect->left() - _lastPoint.x();
        int dy = _bufferedRect->top()  - _lastPoint.y();
        _selectionRect->moveTo(pos.x()+dx, pos.y()+dy);
    }
    else if(_selectionMode == Right) {
        if(pos.x()<=_bufferedRect->left() ) return false;
        _selectionRect->setRight(pos.x());
    }
    else if(_selectionMode == Left) {
         if(pos.x()>=_bufferedRect->right() ) return false;
        _selectionRect->setLeft(pos.x());
    }
    else if(_selectionMode == Top) {
         if(pos.y()>=_bufferedRect->bottom() ) return false;
        _selectionRect->setTop(pos.y());
    }
    else if(_selectionMode == Bottom) {
         if(pos.y()<=_bufferedRect->top() ) return false;
        _selectionRect->setBottom(pos.y());

    }
    return true;
}

void replaceSelectionArea(QImage* prevImage,QImage* image, QRect* prev, QRect* next){
    *image = *prevImage;
    QPainter painter(image);
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(*prev,Qt::transparent); painter.restore();
    painter.drawImage(*next, *prevImage, *prev);
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
        update(_rotation.getUpdateRect());
    }
    else if(_tool == Select || _tool == Paste || _tool == Rectangle || _tool == Ellipse) {

        if( _selectionMode == None) { drawSelectionSquareTo(p); return; }
        if(!resizeSelectionArea(p)) return;
        replaceSelectionArea(_bufferedImage,_image,_bufferedRect,_selectionRect);
        update();

    }


}
void clearArea(QImage& image, const QRect& area){
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(area, Qt::transparent);
}
bool Canvas::setSelectionMode(QPoint pos){
    if(qAbs(pos.x()-_selectionRect->right()) <5){
        setCursor(Qt::SizeHorCursor);
        _selectionMode = Right;
        return true;
    }
    else if(qAbs(pos.x()-_selectionRect->left()) <5){
        setCursor(Qt::SizeHorCursor);
        _selectionMode = Left;
        return true;
    }
    else if(qAbs(pos.y()-_selectionRect->bottom()) <5){
        setCursor(Qt::SizeVerCursor);
        _selectionMode = Bottom;
        return true;
    }
    else if(qAbs(pos.y()-_selectionRect->top()) <5){
        setCursor(Qt::SizeVerCursor);
        _selectionMode = Top;
        return true;
    }
    else if(_selectionRect->contains(pos)){
        setCursor(Qt::SizeAllCursor);
        _selectionMode = Move;
        _lastPoint = pos;
        return true;
    }
    return false;
}

void Canvas::pasteImage(QPoint pos)
{
    if(!hasBufferedArea()) return ;
    QPainter painter(_image);
    QRect updateRect = QRect(pos,_bufferedRect->size());
    painter.drawImage(updateRect, *_bufferedImage, *_bufferedRect);
    _selectionRect = new QRect(updateRect.topLeft(), updateRect.size());
    _tool = Select;
}
void Canvas::prepareForRotation(QPoint pos)
{
    _rotation.setRotatingRect(*_selectionRect);
    _rotation.setCenter(_selectionRect->center());
    _rotation.setBegin(pos);
     clearArea(*_image, *_bufferedRect);
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
    if(_tool == Select || _tool == Rectangle || _tool == Ellipse) {
         _selectionFinished = false;

        if(hasSelectedArea()){
            if(setSelectionMode(p)) return;
            else {
                _selectionMode = None;
                QRect updateRect = (*_selectionRect ).adjusted(-3, -3, +3, +3);
                delete _selectionRect;
                _selectionRect = nullptr;
                update(updateRect);
            }
        }
        _selectionRect = new QRect(event->pos(), QSize(1,1));

    }

    if(_tool == Select) emit enableCutCopy(true);

}
int getIndex(const QImage* im,  QList<Pair<QString,QImage>>&list){
   for(int i=0; i<list.size(); ++i)
       if(&(list[i].second()) == im) return i;
   return -1;
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {

    if(_tool == Select){
        setCursor(Qt::ArrowCursor);
        _selectionMode = None;
        _selectionFinished = true;
        if(hasBufferedArea()) {
            delete _bufferedRect;
            delete _bufferedImage;
        }
        _bufferedRect = new QRect(_selectionRect->topLeft(), _selectionRect->size());
        _bufferedImage = new QImage(*_image);
        update();
        if(_selectionMode == None) return;
    }

    else if(_tool == Rectangle || _tool == Ellipse){
        QPainter painter(_image);
        painter.setPen(QPen(_penColor,_penWidth));
        if(_tool == Rectangle) painter.drawRect(*_selectionRect);
        else  painter.drawEllipse(*_selectionRect);
        update(_selectionRect->adjusted(-3,-3,3,3));
        saveState();
        delete _selectionRect;
        _selectionRect = nullptr;
    }

    else if(_tool == Rotate){
        QPainter painter(_image);
        painter.translate(_rotation.center());
        painter.rotate(_rotation.angle());
        QPoint origin = _rotation.getAfterTransformOriginPoint();
        painter.drawImage(origin, *_bufferedImage);
    }

    if(_tool!= Select && _tool != ColorPicker) saveState();
    int curInd = getIndex(_image,_layers); assert(curInd!=-1);
    const QModelIndex& changed = _model.index(curInd);
    _model.dataChanged(changed, changed);
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
}



void Canvas::resizeEvent(QResizeEvent *event)
{
    if (width() > _image->width() || height() > _image->height()) {
        int newWidth = qMax(width() + 128, _image->width());
        int newHeight = qMax(height() + 128, _image->height());
        resizeImage(_image, QSize(newWidth, newHeight));
        update();
    }
       QWidget::resizeEvent(event);
}

void Canvas::drawLineTo(const QPoint &nextPoint)
{

    QPainter painter(_image);
    if(hasSelectedArea()) painter.setClipRect(*_selectionRect);
    if(_tool == Eraser) painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(getToolColor(),_penWidth));
    painter.drawLine(_lastPoint,nextPoint);

    int rad = (_penWidth / 2) + 2;

    update(QRect(_lastPoint, nextPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));

    _lastPoint = nextPoint;
}

void Canvas::drawSelectionSquareTo(const QPoint &p)
{
    const QPoint& topLeft = _selectionRect->topLeft();
    if(p.x() < topLeft.x() || p.y() < topLeft.y()) return ;

    QRect prev = *_selectionRect;
    _selectionRect->setBottomRight(p);


    update((prev | *_selectionRect)
              .adjusted(-3, -3, +3, +3));

}

void Canvas::resizeImage(QImage *image, const QSize &newSize)
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

    int minX, maxX, minY, maxY;
    if(hasSelectedArea()){
        minX = _selectionRect->left(); maxX = _selectionRect->right()-1;
        minY = _selectionRect->top(); maxY = _selectionRect->bottom()-1;
    }else {
        minX = 0; maxX = _image->width()-1;
        minY = 0; maxY = _image->height()-1;
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
    if(hasSelectedArea()) painter.setClipRect(*_selectionRect);

    painter.setPen(QPen(_penColor,_penWidth/4));
    int num = qrand() % 10;

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
    if(event->key() == Qt::Key_T && _tool == Select && hasSelectedArea())     setTool(Rotate);
    else if(event->key() == Qt::Key_T && _tool == Rotate) setTool(Select);
    qDebug() << (_tool == Rotate);
}
bool Canvas::hasSelectedArea()
{
    return _selectionRect;
}

bool Canvas::hasBufferedArea()
{
    return _bufferedRect;
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
    delete _selectionRect;
    delete _bufferedImage;
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
     if(_selectionRect){
         emit pasteSignal();
         setTool(Paste);
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
