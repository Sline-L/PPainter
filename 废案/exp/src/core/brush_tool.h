#ifndef BRUSH_TOOL_H
#define BRUSH_TOOL_H

#include "drawing_tool.h"

class BrushTool : public DrawingTool
{
    Q_OBJECT

public:
    explicit BrushTool(QObject *parent = nullptr);
    
    void mouse_press(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_move(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_release(const QPoint &pos, PixelCanvas *canvas) override;

private:
    QPoint last_pos_;
    
    // 【扩展点】添加画笔形状、大小等属性
};

#endif // BRUSH_TOOL_H
