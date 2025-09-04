#include "main_window.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QLabel>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , canvas_widget_(nullptr)
    , tool_bar_(nullptr)
    , color_palette_(nullptr)
    , layer_panel_(nullptr)
{
    init_ui();
    setup_menu_bar();
    setup_status_bar();
}

MainWindow::~MainWindow()
{
    // Qt的父子关系会自动处理内存释放
}

void MainWindow::init_ui()
{
    // 设置窗口属性
    setWindowTitle("PPainter - 多人协作像素画板");
    setMinimumSize(1200, 800);
    resize(1600, 1000);

    // 创建中央画布组件
    canvas_widget_ = new CanvasWidget(this);
    setCentralWidget(canvas_widget_);

    // 创建工具栏并添加到窗口
    tool_bar_ = new ToolBar(this);
    addToolBar(Qt::LeftToolBarArea, tool_bar_);

    // 创建调色板停靠窗口
    QDockWidget* color_dock = new QDockWidget("调色板", this);
    color_palette_ = new ColorPalette(this);
    color_dock->setWidget(color_palette_);
    color_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, color_dock);

    // 创建图层面板停靠窗口
    QDockWidget* layer_dock = new QDockWidget("图层", this);
    layer_panel_ = new LayerPanel(this);
    layer_dock->setWidget(layer_panel_);
    layer_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, layer_dock);

    // 设置停靠窗口为标签页模式
    tabifyDockWidget(color_dock, layer_dock);
    color_dock->raise(); // 默认显示调色板

    // 连接信号槽
    connect_signals();
}

void MainWindow::setup_menu_bar()
{
    QMenuBar* menu_bar = menuBar();

    // 文件菜单
    QMenu* file_menu = menu_bar->addMenu("文件(&F)");
    
    QAction* new_action = new QAction("新建(&N)", this);
    new_action->setShortcut(QKeySequence::New);
    new_action->setStatusTip("创建新的像素画");
    connect(new_action, &QAction::triggered, this, &MainWindow::on_new_file);
    file_menu->addAction(new_action);

    QAction* open_action = new QAction("打开(&O)", this);
    open_action->setShortcut(QKeySequence::Open);
    open_action->setStatusTip("打开已有的像素画文件");
    connect(open_action, &QAction::triggered, this, &MainWindow::on_open_file);
    file_menu->addAction(open_action);

    file_menu->addSeparator();

    QAction* save_action = new QAction("保存(&S)", this);
    save_action->setShortcut(QKeySequence::Save);
    save_action->setStatusTip("保存当前像素画");
    connect(save_action, &QAction::triggered, this, &MainWindow::on_save_file);
    file_menu->addAction(save_action);

    QAction* export_action = new QAction("导出图片(&E)", this);
    export_action->setShortcut(QKeySequence("Ctrl+E"));
    export_action->setStatusTip("导出为图片文件");
    connect(export_action, &QAction::triggered, this, &MainWindow::on_export_image);
    file_menu->addAction(export_action);

    file_menu->addSeparator();

    QAction* quit_action = new QAction("退出(&Q)", this);
    quit_action->setShortcut(QKeySequence::Quit);
    quit_action->setStatusTip("退出应用程序");
    connect(quit_action, &QAction::triggered, this, &QWidget::close);
    file_menu->addAction(quit_action);

    // 编辑菜单
    QMenu* edit_menu = menu_bar->addMenu("编辑(&E)");
    
    QAction* undo_action = new QAction("撤销(&U)", this);
    undo_action->setShortcut(QKeySequence::Undo);
    undo_action->setStatusTip("撤销上一步操作");
    edit_menu->addAction(undo_action);

    QAction* redo_action = new QAction("重做(&R)", this);
    redo_action->setShortcut(QKeySequence::Redo);
    redo_action->setStatusTip("重做上一步操作");
    edit_menu->addAction(redo_action);

    edit_menu->addSeparator();

    QAction* clear_action = new QAction("清空画布(&C)", this);
    clear_action->setShortcut(QKeySequence("Ctrl+Shift+N"));
    clear_action->setStatusTip("清空当前画布");
    edit_menu->addAction(clear_action);

    // 视图菜单
    QMenu* view_menu = menu_bar->addMenu("视图(&V)");
    
    QAction* zoom_in_action = new QAction("放大(&I)", this);
    zoom_in_action->setShortcut(QKeySequence::ZoomIn);
    zoom_in_action->setStatusTip("放大画布");
    view_menu->addAction(zoom_in_action);

    QAction* zoom_out_action = new QAction("缩小(&O)", this);
    zoom_out_action->setShortcut(QKeySequence::ZoomOut);
    zoom_out_action->setStatusTip("缩小画布");
    view_menu->addAction(zoom_out_action);

    QAction* reset_zoom_action = new QAction("重置缩放(&R)", this);
    reset_zoom_action->setShortcut(QKeySequence("Ctrl+0"));
    reset_zoom_action->setStatusTip("重置画布缩放到100%");
    view_menu->addAction(reset_zoom_action);

    // 网络菜单
    QMenu* network_menu = menu_bar->addMenu("网络(&N)");
    
    QAction* connect_action = new QAction("连接服务器(&C)", this);
    connect_action->setStatusTip("连接到协作服务器");
    network_menu->addAction(connect_action);

    QAction* disconnect_action = new QAction("断开连接(&D)", this);
    disconnect_action->setStatusTip("断开服务器连接");
    network_menu->addAction(disconnect_action);

    // 帮助菜单
    QMenu* help_menu = menu_bar->addMenu("帮助(&H)");
    
    QAction* about_action = new QAction("关于(&A)", this);
    about_action->setStatusTip("关于像素画板");
    connect(about_action, &QAction::triggered, this, &MainWindow::on_about);
    help_menu->addAction(about_action);
}

void MainWindow::setup_status_bar()
{
    QStatusBar* status_bar = statusBar();
    
    // 创建状态栏标签
    zoom_label_ = new QLabel("缩放: 100%");
    canvas_size_label_ = new QLabel("画布: 64x64");
    cursor_pos_label_ = new QLabel("位置: (0, 0)");
    network_status_label_ = new QLabel("离线");

    // 添加到状态栏
    status_bar->addWidget(zoom_label_);
    status_bar->addPermanentWidget(canvas_size_label_);
    status_bar->addPermanentWidget(cursor_pos_label_);
    status_bar->addPermanentWidget(network_status_label_);

    // 设置默认状态消息
    status_bar->showMessage("就绪", 2000);
}

void MainWindow::connect_signals()
{
    // 连接画布和工具栏的信号槽
    if (canvas_widget_ && tool_bar_) {
        connect(tool_bar_, &ToolBar::tool_changed, 
                canvas_widget_, &CanvasWidget::set_current_tool);
        connect(tool_bar_, &ToolBar::brush_size_changed,
                canvas_widget_, &CanvasWidget::set_brush_size);
    }

    // 连接画布和调色板的信号槽
    if (canvas_widget_ && color_palette_) {
        connect(color_palette_, &ColorPalette::color_changed,
                canvas_widget_, &CanvasWidget::set_current_color);
    }

    // 连接画布状态更新信号
    if (canvas_widget_) {
        connect(canvas_widget_, &CanvasWidget::zoom_changed,
                this, &MainWindow::on_zoom_changed);
        connect(canvas_widget_, &CanvasWidget::cursor_position_changed,
                this, &MainWindow::on_cursor_position_changed);
    }
}

void MainWindow::on_new_file()
{
    if (canvas_widget_) {
        // 如果当前有未保存的更改，询问用户是否保存
        if (canvas_widget_->has_unsaved_changes()) {
            int ret = QMessageBox::question(this, "新建文件",
                "当前文件有未保存的更改，是否保存？",
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            
            if (ret == QMessageBox::Save) {
                on_save_file();
            } else if (ret == QMessageBox::Cancel) {
                return;
            }
        }
        
        canvas_widget_->create_new_canvas(64, 64); // 默认64x64像素
        statusBar()->showMessage("已创建新的像素画", 2000);
    }
}

void MainWindow::on_open_file()
{
    QString file_path = QFileDialog::getOpenFileName(this,
        "打开像素画文件", "", 
        "像素画文件 (*.pxl);;图片文件 (*.png *.jpg *.bmp)");
    
    if (!file_path.isEmpty() && canvas_widget_) {
        if (canvas_widget_->load_from_file(file_path)) {
            statusBar()->showMessage("文件打开成功: " + file_path, 2000);
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件: " + file_path);
        }
    }
}

void MainWindow::on_save_file()
{
    if (!canvas_widget_) return;
    
    QString file_path = QFileDialog::getSaveFileName(this,
        "保存像素画文件", "", "像素画文件 (*.pxl)");
    
    if (!file_path.isEmpty()) {
        if (canvas_widget_->save_to_file(file_path)) {
            statusBar()->showMessage("文件保存成功: " + file_path, 2000);
        } else {
            QMessageBox::warning(this, "错误", "无法保存文件: " + file_path);
        }
    }
}

void MainWindow::on_export_image()
{
    if (!canvas_widget_) return;
    
    QString file_path = QFileDialog::getSaveFileName(this,
        "导出图片", "", 
        "PNG文件 (*.png);;JPEG文件 (*.jpg);;BMP文件 (*.bmp)");
    
    if (!file_path.isEmpty()) {
        if (canvas_widget_->export_to_image(file_path)) {
            statusBar()->showMessage("图片导出成功: " + file_path, 2000);
        } else {
            QMessageBox::warning(this, "错误", "无法导出图片: " + file_path);
        }
    }
}

void MainWindow::on_about()
{
    QMessageBox::about(this, "关于像素画板",
        "<h2>像素画板 v1.0</h2>"
        "<p>一个支持多人在线协作的像素画绘制软件</p>"
        "<p>功能特性：</p>"
        "<ul>"
        "<li>像素级精确绘制</li>"
        "<li>多人实时协作</li>"
        "<li>图层支持</li>"
        "<li>撤销/重做</li>"
        "<li>多种绘图工具</li>"
        "</ul>"
        "<p>使用Qt框架开发</p>");
}

void MainWindow::on_zoom_changed(double zoom_factor)
{
    if (zoom_label_) {
        zoom_label_->setText(QString("缩放: %1%").arg(
            static_cast<int>(zoom_factor * 100)));
    }
}

void MainWindow::on_cursor_position_changed(int x, int y)
{
    if (cursor_pos_label_) {
        cursor_pos_label_->setText(QString("位置: (%1, %2)").arg(x).arg(y));
    }
}

void MainWindow::update_network_status(const QString& status)
{
    if (network_status_label_) {
        network_status_label_->setText(status);
    }
}
