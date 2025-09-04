#ifndef PIXEL_CANVAS_H
#define PIXEL_CANVAS_H

#include <QObject>
#include <QImage>
#include <QColor>

class PixelCanvas : public QObject
{
    Q_OBJECT

public:
    explicit PixelCanvas(QObject *parent = nullptr);
    
    void set_pixel(int x, int y, const QColor &color);
    QColor get_pixel(int x, int y) const;
    
    int width() const { return canvas_width_; }
    int height() const { return canvas_height_; }
    
    const QImage& get_image() const { return canvas_image_; }

signals:
    void canvas_changed();

private:
    QImage canvas_image_;
    int canvas_width_;
    int canvas_height_;
    
    // 【扩展点】添加图层支持
    // 【扩展点】添加历史记录
};

#endif // PIXEL_CANVAS_H
