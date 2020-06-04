#ifndef CANVAS_H
#define CANVAS_H

#include "Layer.h"
#include "layermodel.h"
#include "rotateevent.h"
#include "selectevent.h"

#include <QWidget>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT
    QCursor _ACWCursor;
    QCursor _CWCursor;

public:
    enum Tool { Pen , Bucket, Eraser, Select, Paste, Ellipse, Rectangle, Triangle, Spray, ColorPicker, Rotate };

    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();


    QColor penColor() const;
    int  penWidth() const;
    bool isModified() const;

    bool openImage(const QString& file);
    bool saveImage(const QString& file, const char* fileFormat);
    void setPenColor(const QColor& color);
    void setPenWidth(int width);
    void setTool(const Tool& t);
    QImage getResImage();


public slots:
    void clearImage();
    void redo();
    void undo();
    void cut();
    void copy();
    void paste();
    void setCurrentLayer(const QModelIndex& ind);
    void insertLayer(const QModelIndex& ind);
    void removeLayer(const QModelIndex& ind);
    LayerModel* getModel();
    void addImage(const QString& name, const QImage& im);

signals:
    void undoSignal(bool);
    void redoSignal(bool);
    void pasteSignal();
    void enableCutCopy(bool);
    void layerAdded(QImage* im);
    void layerRemoved(const QModelIndex& ind);
    void redrawLayout(int row);
    void setSelected(const QModelIndex& ind);
    void toolSet(Tool t);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Canvas *ui;
    void drawLineTo(const QPoint& p);
    void drawSelectionSquareTo(const QPoint& p);
    void resizeCanvas(QImage* image, const QSize &newSize);
    void saveState();
    void fill(QPoint pos);
    void drawSpray(QPoint pos);
    void pickColor(QPoint pos);
    void prepareForRotation(QPoint pos);
    bool resizeSelectionArea(QPoint pos);
    bool setSelectionMode(QPoint pos);
    void pasteImage(QPoint pos);
    bool isShapeTool();
    void drawShape(QPainter& p);
    QColor getToolColor();





    LayerModel _model; //model for layerList
    int _c; //counts how many layers were created from the start, used to give name to new layer
    bool _modified; //was canvas content saved to file?
    QPoint _lastPoint; //point when user pressed on canvas
    QList<QPair<QImage*,QImage>> _saves; //used with redo/undo actions
    int _curSave; //index of curent save in _saves
    int _lastAvailableSave; //as data in saves is often rewritten, holds index of last possible save image, is used in redo action



    QImage* _image; // current drawing layer
    QList<Layer> _layers; // all drawing layers
    RotateEvent _rotation; //for Rotate tool
    SelectEvent _selection; // for Select tool
    ShapeEvent _shape;

    QColor _penColor;
    int _penWidth;
    Tool _tool;

};

#endif // CANVAS_H
