#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // 初始化UI组件
    void init_ui();
    // 设置菜单栏
    void setup_menu_bar();
    // 设置状态栏
    void setup_status_bar();
    
private slots:
    // 文件操作槽函数
    void on_new_file();
    void on_open_file();
    void on_save_file();
    void on_export_image();
    
private:
    CanvasWidget* canvas_widget_;
    ToolBar* tool_bar_;
    ColorPalette* color_palette_;
    LayerPanel* layer_panel_;
}

#endif