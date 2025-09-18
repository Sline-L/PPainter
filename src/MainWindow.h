#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QLineEdit>
#include "Imagepaster.h"

class DrawingArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class DrawingArea;
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
    void selectTextTool();
    void selectEraserTool();
    void changePenColor();
    void changePenWidth(int width);
    void about();
    void sendMessage(const QPoint start,const  QPoint end, const QColor color, const int width);
    void connectToServer();
    void disconnectFromServer();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void updateToolStatus();

private:
    DrawingArea *m_drawingArea;
    ImagePaster *m_imagePaster;
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
    QAction *m_textToolAction;
    QAction *m_eraserToolAction;

    // 插图动作
    QAction *m_insertImageToRectAction;
    QAction *m_insertImageAction;
    QAction *m_pasteImageAction;
    QMenu   *m_openMenu;

    // 工具组
    QActionGroup *m_toolGroup;
    
    // 画笔设置控件
    QPushButton *m_colorButton;
    QSpinBox *m_penWidthSpinBox;
    QLabel *m_penWidthLabel;
    
    // 状态栏
    QLabel *m_statusLabel;
    
    QString m_currentFile;

    QTcpSocket *socket;
    QLineEdit *serverAddressInput;
    QLineEdit *serverPortInput;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    // 数据接收缓冲区
    QByteArray receive_buffer;

    // 当前正在接收的数据信息
    enum ReceiveState {
        WaitingForHeader,
        ReceivingMessage,
        ReceivingImage
    };

    ReceiveState current_state;
    qint64 expected_data_size;
    QString current_filename;
    QByteArray current_data_buffer;
    void processReceivedData();
    void resetReceiveState();
};

#endif // MAINWINDOW_H
