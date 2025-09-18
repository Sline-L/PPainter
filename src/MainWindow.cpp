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
#include <QIcon>
#include <QDebug>

using namespace std;

QByteArray colorToByteArray(const QColor &color) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << color;
    return byteArray;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_drawingArea(nullptr)
    , current_state(WaitingForHeader)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_drawingArea = new DrawingArea(centralWidget, this);
    m_imagePaster = new ImagePaster(m_drawingArea);

    serverAddressInput = new QLineEdit(centralWidget);
    serverAddressInput->setPlaceholderText("服务器地址");
    serverAddressInput->setText("127.0.0.1");

    serverPortInput = new QLineEdit(centralWidget);
    serverPortInput->setPlaceholderText("端口");
    serverPortInput->setText("12345");

    connectButton = new QPushButton("连接", centralWidget);
    disconnectButton = new QPushButton("断开", centralWidget);
    disconnectButton->setEnabled(false);

    QHBoxLayout *connectionLayout = new QHBoxLayout();
    connectionLayout->addWidget(serverAddressInput);
    connectionLayout->addWidget(serverPortInput);
    connectionLayout->addWidget(connectButton);
    connectionLayout->addWidget(disconnectButton);

    mainLayout->addLayout(connectionLayout);
    mainLayout->addWidget(m_drawingArea);

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
    qDebug() << "resource exists : /icon/line.png =" << QFile::exists(":/icon/line.png");
    qDebug() << "QIcon isNull for line :" << QIcon(":/icon/line.png").isNull();

    m_newAction = new QAction(QIcon(":/icon/new.png"), tr("新建(&N)"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip(tr("创建新的画布"));
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);
    
    m_openAction = new QAction(QIcon(":/icon/open.png"), tr("打开(&O)"), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("打开已有图片"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);

    m_insertImageAction = new QAction(QIcon(":/icon/paste_image.png"), tr("向区域插入已有图片"), this);
    m_insertImageAction->setStatusTip(tr("在当前画布上插入一张图片（从文件选择）"));
    connect(m_insertImageAction, &QAction::triggered, m_imagePaster, &ImagePaster::chooseImage);

    m_pasteImageAction = new QAction(tr("从剪贴板粘贴图片"), this);
    m_pasteImageAction->setShortcut(QKeySequence::Paste);
    m_pasteImageAction->setShortcutContext(Qt::ApplicationShortcut);
    m_pasteImageAction->setStatusTip(tr("从剪贴板粘贴图片到画布"));
    connect(m_pasteImageAction, &QAction::triggered, m_imagePaster, &ImagePaster::pasteFromClipboard);
    addAction(m_pasteImageAction);

    m_saveAction = new QAction(QIcon(":/icon/save.png"), tr("保存(&S)"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("保存当前画布"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    
    m_saveAsAction = new QAction(tr("另存为(&A)"), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("将画布另存为新文件"));
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    
    m_clearAction = new QAction(QIcon(":/icon/clear.png"), tr("清空画布(&C)"), this);
    m_clearAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Delete));
    m_clearAction->setStatusTip(tr("清空当前画布"));
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::clearCanvas);
    
    m_exitAction = new QAction(tr("退出(&X)"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("退出应用程序"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    // 绘图工具动作
    m_lineToolAction = new QAction(QIcon(":/icon/line.png"), tr("直线工具(&L)"), this);
    m_lineToolAction->setCheckable(true);
    m_lineToolAction->setShortcut(QKeySequence(Qt::Key_L));
    m_lineToolAction->setStatusTip(tr("选择直线绘制工具"));
    connect(m_lineToolAction, &QAction::triggered, this, &MainWindow::selectLinetool);

    m_rectangleToolAction = new QAction(QIcon(":/icon/rect.png"), tr("矩形工具(&R)"), this);
    m_rectangleToolAction->setCheckable(true);
    m_rectangleToolAction->setShortcut(QKeySequence(Qt::Key_R));
    m_rectangleToolAction->setStatusTip(tr("选择矩形绘制工具"));
    connect(m_rectangleToolAction, &QAction::triggered, this, &MainWindow::selectRectangleTool);
    
    m_curveToolAction = new QAction(QIcon(":/icon/curve.png"), tr("曲线工具(&C)"), this);
    m_curveToolAction->setCheckable(true);
    m_curveToolAction->setChecked(true);
    m_curveToolAction->setShortcut(QKeySequence(Qt::Key_C));
    m_curveToolAction->setStatusTip(tr("选择曲线绘制工具"));
    connect(m_curveToolAction, &QAction::triggered, this, &MainWindow::selectCurveTool);

    // 文本工具
    m_textToolAction = new QAction(QIcon(":/icon/text.png"), tr("文本工具(&T)"), this);
    m_textToolAction->setCheckable(true);
    m_textToolAction->setShortcut(QKeySequence(Qt::Key_T));
    m_textToolAction->setStatusTip(tr("插入文本框并输入文本"));
    connect(m_textToolAction, &QAction::triggered, this, &MainWindow::selectTextTool);

    m_eraserToolAction = new QAction(QIcon(":/icon/clear.png"), tr("橡皮擦(&E)"), this);
    m_eraserToolAction->setCheckable(true);
    m_eraserToolAction->setShortcut(QKeySequence(Qt::Key_E));
    m_eraserToolAction->setStatusTip(tr("矩形框选擦除（橡皮擦）"));
    connect(m_eraserToolAction, &QAction::triggered, this, &MainWindow::selectEraserTool);

    // 创建工具组
    m_toolGroup = new QActionGroup(this);
    m_toolGroup->addAction(m_lineToolAction);
    m_toolGroup->addAction(m_rectangleToolAction);
    m_toolGroup->addAction(m_curveToolAction);
    m_toolGroup->addAction(m_textToolAction);
    m_toolGroup->addAction(m_eraserToolAction);

    // 帮助动作
    m_aboutAction = new QAction(QIcon(":/icon/about.png"), tr("关于(&A)"), this);
    m_aboutAction->setStatusTip(tr("显示应用程序信息"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);

    m_insertImageToRectAction = new QAction(QIcon(":/icon/paste_image.png"), tr("插入图片到选区"), this);
    m_insertImageToRectAction->setStatusTip(tr("在选定区域插入已有图片"));
    connect(m_insertImageToRectAction, &QAction::triggered, m_imagePaster, &ImagePaster::chooseImage);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    m_fileMenu->addAction(m_newAction);

    m_openMenu = m_fileMenu->addMenu(tr("打开(&O)"));
    m_openMenu->addAction(m_openAction);
    // 菜单中仍可保留一个文本入口到插入图片（从文件选择）
    m_openMenu->addAction(m_insertImageAction);

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_clearAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_toolMenu = menuBar()->addMenu(tr("工具(&T)"));
    m_toolMenu->addAction(m_lineToolAction);
    m_toolMenu->addAction(m_rectangleToolAction);
    m_toolMenu->addAction(m_curveToolAction);
    m_toolMenu->addAction(m_textToolAction);
    m_toolMenu->addAction(m_eraserToolAction);

    m_helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars()
{
    m_fileToolBar = addToolBar(tr("文件"));
    m_fileToolBar->setIconSize(QSize(32, 32));
    m_fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_clearAction);
    
    m_drawingToolBar = addToolBar(tr("绘图工具"));
    m_drawingToolBar->setIconSize(QSize(32, 32));
    m_drawingToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_drawingToolBar->addAction(m_lineToolAction);
    m_drawingToolBar->addAction(m_rectangleToolAction);
    m_drawingToolBar->addAction(m_curveToolAction);
    m_drawingToolBar->addAction(m_textToolAction);
    m_drawingToolBar->addAction(m_eraserToolAction);
    m_drawingToolBar->addSeparator();

    // 按钮（从文件选择插入）放到工具栏上
    m_drawingToolBar->addAction(m_insertImageAction);
    m_drawingToolBar->addSeparator();

    m_colorButton = new QPushButton(this);
    m_colorButton->setIcon(QIcon(":/icon/color.png"));
    m_colorButton->setFixedSize(40, 40);
    connect(m_colorButton, &QPushButton::clicked, this, &MainWindow::changePenColor);
    m_drawingToolBar->addWidget(m_colorButton);
    
    m_drawingToolBar->addSeparator();
    
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
void MainWindow::selectTextTool()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setToolType(ToolType::Text);
    m_drawingArea->setDrawingTool(tool);
    updateToolStatus();
    statusBar()->showMessage(tr("已选择文本工具"), 2000);
}
void MainWindow::selectEraserTool()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    tool.setToolType(ToolType::Eraser);
    m_drawingArea->setDrawingTool(tool);
    updateToolStatus();
    statusBar()->showMessage(tr("已选择橡皮擦工具"), 2000);
}
void MainWindow::changePenColor()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    QColor currentColor = tool.getPenColor();
    
    QColor newColor = QColorDialog::getColor(currentColor, this, tr("选择画笔颜色"));
    
    if (newColor.isValid() && newColor != currentColor) {
        tool.setPenColor(newColor);
        m_drawingArea->setDrawingTool(tool);
        
        QString colorStyle = QString("QPushButton { background-color: %1; }")
                            .arg(newColor.name());
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
                         "<li>矩形绘制</li>"
                         "<li>文本插入</li>"
                         "<li>自定义画笔颜色和宽度</li>"
                         "<li>保存和加载图片</li>"
                         "<li>清空画布功能</li>"
                         "<li>矩形框选擦除（橡皮擦）</li>"
                         "</ul>"
                         "<p>使用Qt框架开发</p>"));
}
void MainWindow::updateToolStatus()
{
    DrawingTool tool = m_drawingArea->getDrawingTool();
    
    if (tool.getToolType() == ToolType::Line) {
        m_lineToolAction->setChecked(true);
    } 
    else if (tool.getToolType() == ToolType::Rectangle) {
        m_rectangleToolAction->setChecked(true);
    }
    else if (tool.getToolType() == ToolType::Text) {
        m_textToolAction->setChecked(true);
    }
    else if (tool.getToolType() == ToolType::Eraser) {
        m_eraserToolAction->setChecked(true);
    }
    else {
        m_curveToolAction->setChecked(true);
    }
    
    m_penWidthSpinBox->setValue(tool.getPenWidth());
    
    QColor color = tool.getPenColor();
    QString colorStyle = QString("QPushButton { background-color: %1; }")
                        .arg(color.name());
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
                if (receive_buffer.size() >= 9) {
                    QString filename_size_str = QString::fromUtf8(receive_buffer.mid(4, 4));
                    bool ok;
                    int filename_size = filename_size_str.toInt(&ok);

                    if (ok && filename_size > 0) {
                        int total_header_size = 9 + filename_size + 1 + 8 + 1;
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
                break;
            }
            else {
                receive_buffer.clear();
                break;
            }
        }
        else if (current_state == ReceivingMessage) {
            if (receive_buffer.size() >= expected_data_size) {
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
            break;
        }
        else if (current_state == ReceivingImage) {
            if (receive_buffer.size() >= expected_data_size) {
                QByteArray image_data = receive_buffer.left(expected_data_size);

                receive_buffer.remove(0, expected_data_size);
                resetReceiveState();
                continue;
            }
            break;
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
