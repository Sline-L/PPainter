#include "layer_panel.h"
#include <QVBoxLayout>
#include <QListWidget>

using namespace std;

LayerPanel::LayerPanel(QWidget *parent)
    : QWidget(parent)
    , layout_(new QVBoxLayout(this))
    , layer_list_(new QListWidget(this))
{
    layout_->addWidget(layer_list_);
    
    // 添加默认图层
    layer_list_->addItem("Layer 1");
    
    // 连接信号
    connect(layer_list_, &QListWidget::itemSelectionChanged, this, &LayerPanel::on_layer_selected);
    
    // 【扩展点】添加图层操作按钮
    // 【扩展点】添加新建、删除、复制图层功能
}

void LayerPanel::on_layer_selected()
{
    // 【扩展点】处理图层选择逻辑
}
