#ifndef PIXELCANVAS_H
#define PIXELCANVAS_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>
#include <QObject>
class PixelCanvas : public QObject
{
    Q_OBJECT
public:
    // 获取画布尺寸
    QSize get_canvas_size() const;
    // 设置像素点
    void set_pixel(int x, int y, const QColor& color);
    // 获取像素点
    QColor get_pixel(int x, int y) const;
    // 清空画布
    void clear_canvas();
    // 导入图片
    void import_image(const QImage& image, const QPoint& position);
    
signals:
    void canvas_changed(const QRect& dirty_rect);
    
private:
    QImage canvas_image_;
    QSize canvas_size_;
    CommandSystem* command_system_;
};

#endif