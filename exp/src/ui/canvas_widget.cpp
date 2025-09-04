#include "canvas_widget.h"
#include "../core/pixel_canvas.h"
#include <QPainter>
#include <QMouseEvent>

using namespace std;

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent)
    , pixel_canvas_(new PixelCanvas(this))
{
    setMinimumSize(400, 300);
    // 【扩展点】设置更多widget属性
}

CanvasWidget::~CanvasWidget()
{
    delete pixel_canvas_;
}

void CanvasWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    // 绘制背景
    painter.fillRect(rect(), Qt::white);
    
    // 【扩展点】绘制网格
    // 【扩展点】应用视图变换
    // 【扩展点】绘制像素画布内容
    // 【扩展点】绘制工具预览
    
    QWidget::paintEvent(event);
}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    // 【扩展点】处理鼠标按下事件
    // 【扩展点】转换坐标到画布坐标系
    // 【扩展点】调用当前工具的处理方法
    
    QWidget::mousePressEvent(event);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 【扩展点】处理鼠标移动事件
    QWidget::mouseMoveEvent(event);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // 【扩展点】处理鼠标释放事件
    QWidget::mouseReleaseEvent(event);
}

void CanvasWidget::wheelEvent(QWheelEvent *event)
{
    // 【扩展点】处理滚轮缩放
    QWidget::wheelEvent(event);
}
