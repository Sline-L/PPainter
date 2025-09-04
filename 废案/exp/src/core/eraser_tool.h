#ifndef ERASER_TOOL_H
#define ERASER_TOOL_H

#include "drawing_tool.h"

class EraserTool : public DrawingTool
{
    Q_OBJECT

public:
    explicit EraserTool(QObject *parent = nullptr);
    
    void mouse_press(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_move(const QPoint &pos, PixelCanvas *canvas) override;
    void mouse_release(const QPoint &pos, PixelCanvas *canvas) override;

private:
    // 【扩展点】添加橡皮擦大小等属性
};

#endif // ERASER_TOOL_H
