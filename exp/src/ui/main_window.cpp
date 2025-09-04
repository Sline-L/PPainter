#include "main_window.h"
#include "canvas_widget.h"
#include "tool_bar.h"
#include "color_palette.h"
#include "layer_panel.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , canvas_widget_(nullptr)
    , tool_bar_(nullptr)
    , color_palette_(nullptr)
    , layer_panel_(nullptr)
{
    setup_ui();
}

MainWindow::~MainWindow()
{
    // Qt会自动清理子组件
}

void MainWindow::setup_ui()
{
    // 创建中央widget
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    
    // 创建主布局
    QHBoxLayout *main_layout = new QHBoxLayout(central_widget);
    
    // 创建组件
    tool_bar_ = new ToolBar(this);
    canvas_widget_ = new CanvasWidget(this);
    color_palette_ = new ColorPalette(this);
    layer_panel_ = new LayerPanel(this);
    
    // 创建左侧面板
    QVBoxLayout *left_panel = new QVBoxLayout();
    left_panel->addWidget(tool_bar_);
    left_panel->addWidget(color_palette_);
    left_panel->addStretch();
    
    // 创建右侧面板
    QVBoxLayout *right_panel = new QVBoxLayout();
    right_panel->addWidget(layer_panel_);
    right_panel->addStretch();
    
    // 添加到主布局
    main_layout->addLayout(left_panel);
    main_layout->addWidget(canvas_widget_, 1); // 画布占据主要空间
    main_layout->addLayout(right_panel);
    
    // 【扩展点】添加菜单栏、工具栏、状态栏等
    // 【扩展点】连接信号槽
    // 【扩展点】设置窗口属性
    
    setWindowTitle("Pixel Whiteboard");
    resize(1200, 800);
}
