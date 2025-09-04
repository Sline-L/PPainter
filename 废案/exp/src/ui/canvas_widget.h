#ifndef CANVAS_WIDGET_H
#define CANVAS_WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

class PixelCanvas;

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    ~CanvasWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    
    // 【扩展点】添加更多事件处理方法

private:
    PixelCanvas *pixel_canvas_;
    
    // 【扩展点】添加视图变换相关变量
    // 【扩展点】添加缩放、平移相关变量
};

#endif // CANVAS_WIDGET_H
