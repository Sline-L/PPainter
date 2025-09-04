#ifndef LAYER_PANEL_H
#define LAYER_PANEL_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>

class LayerPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LayerPanel(QWidget *parent = nullptr);

private slots:
    void on_layer_selected();
    // 【扩展点】添加图层操作槽函数

private:
    QVBoxLayout *layout_;
    QListWidget *layer_list_;
    // 【扩展点】添加图层操作按钮
};

#endif // LAYER_PANEL_H
