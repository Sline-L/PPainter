#include "tool_bar.h"
#include <QVBoxLayout>
#include <QPushButton>

using namespace std;

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
    , layout_(new QVBoxLayout(this))
    , brush_button_(new QPushButton("Brush", this))
    , eraser_button_(new QPushButton("Eraser", this))
{
    setup_tools();
}

void ToolBar::setup_tools()
{
    layout_->addWidget(brush_button_);
    layout_->addWidget(eraser_button_);
    layout_->addStretch();
    
    // 连接信号
    connect(brush_button_, &QPushButton::clicked, this, &ToolBar::on_tool_selected);
    connect(eraser_button_, &QPushButton::clicked, this, &ToolBar::on_tool_selected);
    
    // 【扩展点】添加更多工具按钮
    // 【扩展点】设置按钮样式
    // 【扩展点】设置按钮图标
}

void ToolBar::on_tool_selected()
{
    // 【扩展点】处理工具选择逻辑
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        // 处理工具切换
    }
}
