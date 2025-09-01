#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include <QLine>
#include <QPainterPath>

#include "drawing_tool.h"
class BrushTool : public DrawingTool
{
    Q_OBJECT
public:
    void mouse_press(const QPoint& pos, Qt::MouseButton button) override;
    void mouse_move(const QPoint& pos) override;
    void mouse_release(const QPoint& pos) override;
    QCursor get_cursor() const override;
    
    // 设置画笔大小
    void set_brush_size(int size);
    // 设置画笔形状
    void set_brush_shape(BrushShape shape);
    
private:
    void draw_line(const QPoint& from, const QPoint& to);
    void draw_pixel_circle(const QPoint& center, int radius);
    
    QPoint last_pos_;
    bool is_drawing_;
    BrushShape brush_shape_;
};

#endif