#include "pixel_canvas.h"

using namespace std;

PixelCanvas::PixelCanvas(QObject *parent)
    : QObject(parent)
    , canvas_width_(800)
    , canvas_height_(600)
{
    canvas_image_ = QImage(canvas_width_, canvas_height_, QImage::Format_ARGB32);
    canvas_image_.fill(Qt::white);
}

void PixelCanvas::set_pixel(int x, int y, const QColor &color)
{
    if (x >= 0 && x < canvas_width_ && y >= 0 && y < canvas_height_) {
        canvas_image_.setPixelColor(x, y, color);
        emit canvas_changed();
    }
}

QColor PixelCanvas::get_pixel(int x, int y) const
{
    if (x >= 0 && x < canvas_width_ && y >= 0 && y < canvas_height_) {
        return canvas_image_.pixelColor(x, y);
    }
    return QColor();
}
