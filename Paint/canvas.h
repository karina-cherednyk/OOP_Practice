#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

namespace Ui {
class Canvas;
}

class Canvas : public QWidget
{
    Q_OBJECT

    enum SelectionMode{ Left, Right, Top, Bottom, Move,None };

public:
    enum Tool { Pen , Bucket, Eraser, Select, Paste, Ellipse, Rectangle, Spray };

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


signals:
    void undoSignal(bool);
    void redoSignal(bool);
    void pasteSignal();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

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
    QColor getToolColor();

    QColor _penColor;
    QImage _image;
    QPoint _lastPoint;
    bool _modified;
    int _penWidth;
    QList<QImage> _saves;
    int _curSave;
    int _lastAvailableSave;
    Tool _tool;
    QRect* _selectionRect;
    QRect* _bufferedRect;
    QImage* _bufferedImage;
    bool _selectionFinished;
    SelectionMode _selectionMode;
};

#endif // CANVAS_H
