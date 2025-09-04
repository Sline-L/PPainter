#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);

private slots:
    void on_tool_selected();
    // 【扩展点】添加更多工具选择槽函数

private:
    void setup_tools();
    
private:
    QVBoxLayout *layout_;
    QPushButton *brush_button_;
    QPushButton *eraser_button_;
    // 【扩展点】添加更多工具按钮
};

#endif // TOOL_BAR_H
