#include "MainWindow.h"
#include "DrawingArea.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QLineEdit>
#include <QStandardPaths>
#include <QTcpSocket>

using namespace std;

QByteArray colorToByteArray(const QColor &color) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << color; // 使用 QDataStream 输出操作符序列化 QColor
    return byteArray;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_drawingArea(nullptr)
    , current_state(WaitingForHeader)
{
    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建绘图区域
    m_drawingArea = new DrawingArea(centralWidget, this);
    m_imagePaster = new ImagePaster(m_drawingArea);

    // 创建网络连接控件
    serverAddressInput = new QLineEdit(centralWidget);
    serverAddressInput->setPlaceholderText("服务器地址");
    serverAddressInput->setText("127.0.0.1");

    serverPortInput = new QLineEdit(centralWidget);
    serverPortInput->setPlaceholderText("端口");
    serverPortInput->setText("12345");

    connectButton = new QPushButton("连接", centralWidget);
    disconnectButton = new QPushButton("断开", centralWidget);
    disconnectButton->setEnabled(false);

    // 创建连接布局
    QHBoxLayout *connectionLayout = new QHBoxLayout();
    connectionLayout->addWidget(serverAddressInput);
    connectionLayout->addWidget(serverPortInput);
    connectionLayout->addWidget(connectButton);
    connectionLayout->addWidget(disconnectButton);

    // 将连接布局和画布添加到主布局
    mainLayout->addLayout(connectionLayout);
    mainLayout->addWidget(m_drawingArea);

    // 设置中央部件
    setCentralWidget(centralWidget);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setWindowTitle(tr("电子白板"));
    setMinimumSize(1000, 700);
    resize(1200, 800);
    socket = new QTcpSocket(this);

    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &MainWindow::onErrorOccurred);

    updateToolStatus();
}

void MainWindow::createActions()
{
    // 文件动作
    m_newAction = new QAction(tr("新建(&N)"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip(tr("创建新的画布"));
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);
    
    m_openAction = new QAction(tr("打开(&O)"), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("打开已有图片"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
           //新加内容
    m_insertImageAction = new QAction(tr("向区域插入已有图片"), this);
    m_insertImageAction->setStatusTip(tr("在当前画布上插入一张图片"));
    connect(m_insertImageAction,&QAction::triggered,m_imagePaster,&ImagePaster::chooseImage);

    m_saveAction = new QAction(tr("保存(&S)"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("保存当前画布"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    
    m_saveAsAction = new QAction(tr("另存为(&A)"), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("将画布另存为新文件"));
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    
    m_clearAction = new QAction(tr("清空画布(&C)"), this);
    m_clearAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Delete));
    m_clearAction->setStatusTip(tr("清空当前画布"));
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::clearCanvas);
    
    m_exitAction = new QAction(tr("退出(&X)"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("退出应用程序"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    // 绘图工具动作
    m_lineToolAction = new QAction(tr("直线工具(&L)"), this);
    m_lineToolAction->setCheckable(true);
    m_lineToolAction->setShortcut(QKeySequence(Qt::Key_L));
    m_lineToolAction->setStatusTip(tr("选择直线绘制工具"));
    connect(m_lineToolAction, &QAction::triggered, this, &MainWindow::selectLinetool);

    m_rectangleToolAction = new QAction(tr("矩形工具(&R)"), this);
    m_rectangleToolAction->setCheckable(true);
    m_rectangleToolAction->setShortcut(QKeySequence(Qt::Key_R));
    m_rectangleToolAction->setStatusTip(tr("选择矩形绘制工具"));
    connect(m_rectangleToolAction, &QAction::triggered, this, &MainWindow::selectRectangleTool);
    
    m_curveToolAction = new QAction(tr("曲线工具(&C)"), this);
    m_curveToolAction->setCheckable(true);
    m_curveToolAction->setChecked(true);
    m_curveToolAction->setShortcut(QKeySequence(Qt::Key_C));
    m_curveToolAction->setStatusTip(tr("选择曲线绘制工具"));
    connect(m_curveToolAction, &QAction::triggered, this, &MainWindow::selectCurveTool);
    
    // 创建工具组
    m_toolGroup = new QActionGroup(this);
    m_toolGroup->addAction(m_lineToolAction);
    m_toolGroup->addAction(m_rectangleToolAction);
    m_toolGroup->addAction(m_curveToolAction);
    
    // 帮助动作
    m_aboutAction = new QAction(tr("关于(&A)"), this);
    m_aboutAction->setStatusTip(tr("显示应用程序信息"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    // 文件菜单
    m_fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    m_fileMenu->addAction(m_newAction);

    m_openMenu = m_fileMenu->addMenu(tr("打开(&O)"));
    // 打开子菜单里放两个动作
    m_openMenu->addAction(m_openAction);
    // 新增的“打开图片”动作
    m_openMenu->addAction(m_insertImageAction);

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_clearAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    // 工具菜单
    m_toolMenu = menuBar()->addMenu(tr("工具(&T)"));
    m_toolMenu->addAction(m_lineToolAction);
    m_toolMenu->addAction(m_rectangleToolAction);
    m_toolMenu->addAction(m_curveToolAction);
    
    // 帮助菜单
    m_helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    m_helpMenu->addAction(m_aboutAction);
}
void MainWindow::createToolBars()
{
    // 文件工具栏
    m_fileToolBar = addToolBar(tr("文件"));
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_clearAction);
    
    // 绘图工具栏
    m_drawingToolBar = addToolBar(tr("绘图工具"));
    m_drawingToolBar->addAction(m_lineToolAction);
    m_drawingToolBar->addAction(m_rectangleToolAction);
    m_drawingToolBar->addAction(m_curveToolAction);
    m_drawingToolBar->addSeparator();
    
    // 颜色选择按钮
    m_colorButton = new QPushButton(this);
    m_colorButton->setText(tr("颜色"));
    m_colorButton->setFixedSize(60, 30);
    m_colorButton->setStyleSheet("QPushButton { background-color: black; color: white; }");
    connect(m_colorButton, &QPushButton::clicked, this, &MainWindow::changePenColor);
    m_drawingToolBar->addWidget(m_colorButton);
    
    m_drawingToolBar->addSeparator();
    
    // 画笔宽度设置
    m_penWidthLabel = new QLabel(tr("画笔宽度:"), this);
    m_drawingToolBar->addWidget(m_penWidthLabel);
    
    m_penWidthSpinBox = new QSpinBox(this);
    m_penWidthSpinBox->setRange(1, 50);
    m_penWidthSpinBox->setValue(2);
    m_penWidthSpinBox->setSuffix(tr(" px"));
    connect(m_penWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::changePenWidth);
    m_drawingToolBar->addWidget(m_penWidthSpinBox);
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel(tr("就绪"), this);
    statusBar()->addWidget(m_statusLabel);
    statusBar()->showMessage(tr("欢迎使用电子白板"), 2000);
}

void MainWindow::newFile()
{
    if (m_drawingArea) {
        m_drawingArea->clearCanvas();
        m_currentFile.clear();
        setWindowTitle(tr("电子白板 - 新建"));
        statusBar()->showMessage(tr("创建新画布"), 2000);
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("打开图片文件"),
                                   QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                   tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"));
    
    if (!fileName.isEmpty()) {
        m_drawingArea->loadImage(fileName);
        m_currentFile = fileName;
        setWindowTitle(tr("电子白板 - %1").arg(QFileInfo(fileName).fileName()));
        statusBar()->showMessage(tr("已打开: %1").arg(fileName), 2000);
    }
}
void MainWindow::saveFile()
{
    if (m_currentFile.isEmpty()) {
        saveAsFile();
    } else {
        m_drawingArea->saveImage(m_currentFile);
        statusBar()->showMessage(tr("已保存: %1").arg(m_currentFile), 2000);
    }
}
void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                   tr("保存图片"),
                                   QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                   tr("PNG 文件 (*.png);;JPEG 文件 (*.jpg);;BMP 文件 (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        m_drawingArea->saveImage(fileName);
        m_currentFile = fileName;
        setWindowTitle(tr("电子白板 - %1").arg(QFileInfo(fileName).fileName()));
        statusBar()->showMessage(tr("已保存: %1").arg(fileName), 2000);
    }
}
void MainWindow::clearCanvas()
{
    int ret = QMessageBox::question(this, tr("清空画布"),
                                   tr("确定要清空当前画布吗？\n此操作无法撤销。"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_drawingArea->clearCanvas();
        statusBar()->showMessage(tr("画布已清空"), 2000);
    }
}
void MainWindow::selectLinetool()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setToolType(ToolType::Line);
    m_drawingArea->setDrawingTool(tool);
    updateToolStatus();
    statusBar()->showMessage(tr("已选择直线工具"), 2000);
}
void MainWindow::selectRectangleTool()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setToolType(ToolType::Rectangle);
    m_drawingArea->setDrawingTool(tool);
    updateToolStatus();
    statusBar()->showMessage(tr("已选择矩形工具"), 2000);
}
void MainWindow::selectCurveTool()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setToolType(ToolType::Curve);
    m_drawingArea->setDrawingTool(tool);
    updateToolStatus();
    statusBar()->showMessage(tr("已选择曲线工具"), 2000);
}
void MainWindow::changePenColor()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    QColor currentColor = tool.getPenColor();
    
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("选择画笔颜色"));
    
    if (newColor.isValid() && newColor != currentColor) {
        tool.setPenColor(newColor);
        m_drawingArea->setDrawingTool(tool);
        
        // 更新颜色按钮的显示
        QString colorStyle = QString("QPushButton { background-color: %1; color: %2; }")
                            .arg(newColor.name())
                            .arg(newColor.lightness() > 128 ? "black" : "white");
        m_colorButton->setStyleSheet(colorStyle);
        
        statusBar()->showMessage(tr("画笔颜色已更改"), 2000);
    }
}

void MainWindow::changePenWidth(int width)
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setPenWidth(width);
    m_drawingArea->setDrawingTool(tool);
    statusBar()->showMessage(tr("画笔宽度已设置为 %1 像素").arg(width), 2000);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于电子白板"),
                      tr("<h2>电子白板 v1.0</h2>"
                         "<p>这是一个简单的电子白板应用程序，支持：</p>"
                         "<ul>"
                         "<li>直线和曲线绘制</li>"
                         "<li>自定义画笔颜色和宽度</li>"
                         "<li>保存和加载图片</li>"
                         "<li>清空画布功能</li>"
                         "</ul>"
                         "<p>使用Qt框架开发</p>"));
}
void MainWindow::updateToolStatus()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    
    // 更新工具按钮状态
    if (tool.getToolType() == ToolType::Line) {
        m_lineToolAction->setChecked(true);
    } 
    else if (tool.getToolType() == ToolType::Rectangle) {
        m_rectangleToolAction->setChecked(true);
    }
    else {
        m_curveToolAction->setChecked(true);
    }
    
    // 更新画笔宽度显示
    m_penWidthSpinBox->setValue(tool.getPenWidth());
    
    // 更新颜色按钮显示
    QColor color = tool.getPenColor();
    QString colorStyle = QString("QPushButton { background-color: %1; color: %2; }")
                        .arg(color.name())
                        .arg(color.lightness() > 128 ? "black" : "white");
    m_colorButton->setStyleSheet(colorStyle);
}


void MainWindow::connectToServer()
{
    QString address = serverAddressInput->text();
    quint16 port = serverPortInput->text().toUShort();
    socket->connectToHost(address, port);
}

void MainWindow::disconnectFromServer()
{
    socket->disconnectFromHost();
}


void MainWindow::sendMessage(const QPoint start, const QPoint end, const QColor color, const int width)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        return;
    }
    QByteArray arr;
    QByteArray packet;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << start  << end << color << width;
    packet.append("LIN|");
    packet.append(QString("%1").arg(arr.size(), 8, 10, QChar('0')).toUtf8());
    packet.append("|");
    packet.append(arr);
    socket->write(packet);
}

/*
void MainWindow::sendImage()
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    QString file_path = QFileDialog::getOpenFileName(this, "选择图片", "", "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (file_path.isEmpty()) return;

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray image_data = file.readAll();
    file.close();

    QString file_name = QFileInfo(file_path).fileName();

    // 改进的协议：IMG|filename_size_in_4_bytes|filename|image_size_in_8_bytes|image_data
    QByteArray filename_data = file_name.toUtf8();
    QByteArray packet;

    packet.append("IMG|");
    packet.append(QString("%1").arg(filename_data.size(), 4, 10, QChar('0')).toUtf8());
    packet.append("|");
    packet.append(filename_data);
    packet.append("|");
    packet.append(QString("%1").arg(image_data.size(), 8, 10, QChar('0')).toUtf8());
    packet.append("|");
    packet.append(image_data);

    socket->write(packet);

    // 🆕 新增：在本地也显示发送的图片，便于调试
}
*/


void MainWindow::onConnected()
{
    qDebug() << "已连接到服务器";
    connectButton->setEnabled(false);
    disconnectButton->setEnabled(true);
    serverAddressInput->setEnabled(false);
    serverPortInput->setEnabled(false);
}

void MainWindow::onDisconnected()
{
    qDebug() << "已断开与服务器的连接";
    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
    serverAddressInput->setEnabled(true);
    serverPortInput->setEnabled(true);

    resetReceiveState();
}

void MainWindow::onReadyRead()
{
    receive_buffer.append(socket->readAll());
    processReceivedData();
}


void MainWindow::processReceivedData()
{

    while (!receive_buffer.isEmpty()) {
        if (current_state == WaitingForHeader) {
            if (receive_buffer.startsWith("LIN|")) {
                if (receive_buffer.size() >= 13) {
                    QString size_str = QString::fromUtf8(receive_buffer.mid(4, 8));
                    bool ok;
                    qint64 message_size = size_str.toLongLong(&ok);

                    if (ok && message_size >= 0) {
                        current_state = ReceivingMessage;
                        expected_data_size = message_size;
                        receive_buffer.remove(0, 13);
                        continue;
                    }
                }
                break;
            }
            else if (receive_buffer.startsWith("IMG|")) {
                if (receive_buffer.size() >= 9) { // "IMG|" + 4位数字 + "|"
                    QString filename_size_str = QString::fromUtf8(receive_buffer.mid(4, 4));
                    bool ok;
                    int filename_size = filename_size_str.toInt(&ok);

                    if (ok && filename_size > 0) {
                        // 检查是否有完整的文件名和图片大小头部
                        int total_header_size = 9 + filename_size + 1 + 8 + 1; // IMG|xxxx|filename|xxxxxxxx|
                        if (receive_buffer.size() >= total_header_size) {
                            current_filename = QString::fromUtf8(receive_buffer.mid(9, filename_size));

                            QString image_size_str = QString::fromUtf8(receive_buffer.mid(9 + filename_size + 1, 8));
                            qint64 image_size = image_size_str.toLongLong(&ok);

                            if (ok && image_size > 0) {
                                current_state = ReceivingImage;
                                expected_data_size = image_size;
                                receive_buffer.remove(0, total_header_size);

                                continue;
                            }
                        }
                    }
                }
                break; // 等待更多数据
            }
            else {
                // 未知格式，清除错误数据
                receive_buffer.clear();
                break;
            }
        }
        else if (current_state == ReceivingMessage) {
            if (receive_buffer.size() >= expected_data_size) {
                // 接收完整消息
                QByteArray message_data = receive_buffer.left(expected_data_size);
                receive_buffer.remove(0, expected_data_size);
                char split;
                QPoint start, end;
                QColor color;
                int width;

                QDataStream stream(message_data);
                stream >> start;
                stream >> end;
                stream >> color;
                stream >> width;
                m_drawingArea->drawLine(start, end, color, width);
                current_state = WaitingForHeader;
                continue;
            }
            break; // 等待更多数据
        }
        else if (current_state == ReceivingImage) {
            if (receive_buffer.size() >= expected_data_size) {
                // 接收完整图片
                QByteArray image_data = receive_buffer.left(expected_data_size);

                receive_buffer.remove(0, expected_data_size);
                resetReceiveState();
                continue;
            }
            break; // 等待更多数据
        }
    }
}


void MainWindow::resetReceiveState()
{
    current_state = WaitingForHeader;
    expected_data_size = 0;
    current_filename.clear();
    current_data_buffer.clear();
    receive_buffer.clear();
}

void MainWindow::onErrorOccurred(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    qDebug() << "网络错误: " << socket->errorString();
    resetReceiveState();
}
