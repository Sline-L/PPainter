#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CanvasWidget;
class ToolBar;
class ColorPalette;
class LayerPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setup_ui();
    // 【扩展点】添加菜单栏设置、状态栏设置等方法
    
private:
    CanvasWidget *canvas_widget_;
    ToolBar *tool_bar_;
    ColorPalette *color_palette_;
    LayerPanel *layer_panel_;
    
    // 【扩展点】添加更多UI组件指针
};

#endif // MAIN_WINDOW_H
