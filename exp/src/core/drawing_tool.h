#ifndef DRAWING_TOOL_H
#define DRAWING_TOOL_H

#include <QObject>
#include <QPoint>
#include <QColor>

class PixelCanvas;

class DrawingTool : public QObject
{
    Q_OBJECT

public:
    explicit DrawingTool(QObject *parent = nullptr);
    virtual ~DrawingTool() = default;
    
    virtual void mouse_press(const QPoint &pos, PixelCanvas *canvas) = 0;
    virtual void mouse_move(const QPoint &pos, PixelCanvas *canvas) = 0;
    virtual void mouse_release(const QPoint &pos, PixelCanvas *canvas) = 0;
    
    void set_color(const QColor &color) { color_ = color; }
    QColor color() const { return color_; }

protected:
    QColor color_;
    bool is_drawing_;
    
    // 【扩展点】添加画笔大小、透明度等属性
};

#endif // DRAWING_TOOL_H
