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
#include "DrawingArea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void clearCanvas();
    void selectLinetool();
    void selectCurveTool();
    void selectRectangleTool();
    void changePenColor();
    void changePenWidth(int width);
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void updateToolStatus();

private:
    DrawingArea *m_drawingArea;
    
    // 菜单
    QMenu *m_fileMenu;
    QMenu *m_toolMenu;
    QMenu *m_helpMenu;
    
    // 工具栏
    QToolBar *m_fileToolBar;
    QToolBar *m_drawingToolBar;
    
    // 动作
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_clearAction;
    QAction *m_exitAction;
    QAction *m_lineToolAction;
    QAction *m_curveToolAction;
    QAction *m_rectangleToolAction;
    QAction *m_aboutAction;
    
    // 工具组
    QActionGroup *m_toolGroup;
    
    // 画笔设置控件
    QPushButton *m_colorButton;
    QSpinBox *m_penWidthSpinBox;
    QLabel *m_penWidthLabel;
    
    // 状态栏
    QLabel *m_statusLabel;
    
    QString m_currentFile;
};

#endif // MAINWINDOW_H
