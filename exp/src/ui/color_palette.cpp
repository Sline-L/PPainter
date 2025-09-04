#include "color_palette.h"
#include <QPainter>
#include <QMouseEvent>

using namespace std;

ColorPalette::ColorPalette(QWidget *parent)
    : QWidget(parent)
    , current_color_(Qt::black)
{
    setFixedSize(200, 150);
    // 【扩展点】初始化调色板颜色
}

void ColorPalette::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    // 绘制当前颜色
    painter.fillRect(10, 10, 50, 30, current_color_);
    painter.drawRect(10, 10, 50, 30);
    
    // 【扩展点】绘制调色板网格
    // 【扩展点】绘制预设颜色
    
    QWidget::paintEvent(event);
}

void ColorPalette::mousePressEvent(QMouseEvent *event)
{
    // 【扩展点】处理颜色选择
    // 【扩展点】发射颜色改变信号
    
    QWidget::mousePressEvent(event);
}
