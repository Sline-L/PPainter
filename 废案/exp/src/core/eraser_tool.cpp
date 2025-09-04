#include "eraser_tool.h"
#include "pixel_canvas.h"

using namespace std;

EraserTool::EraserTool(QObject *parent)
    : DrawingTool(parent)
{
}

void EraserTool::mouse_press(const QPoint &pos, PixelCanvas *canvas)
{
    is_drawing_ = true;
    canvas->set_pixel(pos.x(), pos.y(), Qt::white); // 用白色擦除
}

void EraserTool::mouse_move(const QPoint &pos, PixelCanvas *canvas)
{
    if (is_drawing_) {
        canvas->set_pixel(pos.x(), pos.y(), Qt::white);
    }
}

void EraserTool::mouse_release(const QPoint &pos, PixelCanvas *canvas)
{
    is_drawing_ = false;
}
