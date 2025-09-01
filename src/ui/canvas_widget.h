#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

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

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    // 设置缩放级别
    void set_zoom_level(double zoom);
    // 设置当前工具
    void set_current_tool(DrawingTool* tool);
    
protected:
    // 重写绘制事件
    void paintEvent(QPaintEvent* event) override;
    // 重写鼠标事件
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    // 重写滚轮事件
    void wheelEvent(QWheelEvent* event) override;
    
private:
    PixelCanvas* pixel_canvas_;
    DrawingTool* current_tool_;
    double zoom_level_;
    QPoint canvas_offset_;
    bool is_dragging_;
    QPoint last_drag_pos_;
};

#endif