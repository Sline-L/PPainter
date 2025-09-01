#ifndef DRAWINGTOOL_H
#define DRAWINGTOOL_H

#include <QObject>

class DrawingTool : public QObject
{
    Q_OBJECT
public:
    virtual void mouse_press(const QPoint& pos, Qt::MouseButton button) = 0;
    virtual void mouse_move(const QPoint& pos) = 0;
    virtual void mouse_release(const QPoint& pos) = 0;
    virtual QCursor get_cursor() const = 0;
    
protected:
    PixelCanvas* canvas_;
    QColor current_color_;
    int brush_size_;
};

#endif