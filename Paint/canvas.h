#ifndef CANVAS_H
#define CANVAS_H

#include "Pair.h"
#include "layermodel.h"
#include "rotateevent.h"

#include <QWidget>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT


public:
    enum Tool { Pen , Bucket, Eraser, Select, Paste, Ellipse, Rectangle, Spray, ColorPicker, Rotate };

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


signals:
    void undoSignal(bool);
    void redoSignal(bool);
    void pasteSignal();
    void enableCutCopy(bool);
    void layerAdded(QImage* im);
    void layerRemoved(const QModelIndex& ind);
    void redrawLayout(int row);
    void setSelected(const QModelIndex& ind);

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
    void resizeImage(QImage* image, const QSize &newSize);
    void saveState();
    void fill(QPoint pos);
    bool hasSelectedArea();
    bool hasBufferedArea();
    void drawSpray(QPoint pos);
    void pickColor(QPoint pos);
    void prepareForRotation(QPoint pos);
    bool resizeSelectionArea(QPoint pos);
    bool setSelectionMode(QPoint pos);
    void pasteImage(QPoint pos);
    QColor getToolColor();



    enum SelectionMode{ Left, Right, Top, Bottom, Move,None }; //for Selection tool area transformations

    LayerModel _model; //model for layerList
    int _c; //counts how many layers were created from the start, used to give name to new layer
    bool _modified; //was canvas content saved to file?
    QPoint _lastPoint; //point when user pressed on canvas
    QList<QPair<QImage*,QImage>> _saves; //used with redo/undo actions
    int _curSave; //index of curent save in _saves
    int _lastAvailableSave; //as data in saves is often rewritten, holds index of last possible save image, is used in redo action

    QRect* _selectionRect; // 1st step - select area, _bufferedRect = _selectionRect; 2nd step - resize/ move area
    QRect* _bufferedRect; // == _selectionRect before resizing
    QImage* _bufferedImage; // == _bufferedRect area of image
    bool _selectionFinished; // when area is selected, 2nd stage - move and resize it

    QImage* _image; // current drawing layer
    QList<Pair<QString,QImage>> _layers; // all drawing layers
    RotateEvent _rotation; //for Rotate tool

    QColor _penColor;
    int _penWidth;
    Tool _tool;
    SelectionMode _selectionMode;
};

#endif // CANVAS_H
