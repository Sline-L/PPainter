#include "brush_tool.h"
#include "pixel_canvas.h"

using namespace std;

BrushTool::BrushTool(QObject *parent)
    : DrawingTool(parent)
{
}

void BrushTool::mouse_press(const QPoint &pos, PixelCanvas *canvas)
{
    is_drawing_ = true;
    last_pos_ = pos;
    canvas->set_pixel(pos.x(), pos.y(), color_);
}

void BrushTool::mouse_move(const QPoint &pos, PixelCanvas *canvas)
{
    if (is_drawing_) {
        canvas->set_pixel(pos.x(), pos.y(), color_);
        last_pos_ = pos;
        // 【扩展点】实现线条插值，避免断点
    }
}

void BrushTool::mouse_release(const QPoint &pos, PixelCanvas *canvas)
{
    is_drawing_ = false;
    // 【扩展点】添加到命令历史
}
