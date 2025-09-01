# PPainter Project

## 工程结构

PixelWhiteboard/  
├── CMakeLists.txt  
├── main.cpp  
├── src/  
│   ├── ui/  
│   │   ├── main_window.h/.cpp              # 主窗口  
│   │   ├── canvas_widget.h/.cpp            # 画布组件  
│   │   ├── tool_bar.h/.cpp                 # 工具栏  
│   │   ├── color_palette.h/.cpp            # 调色板  
│   │   └── layer_panel.h/.cpp              # 图层面板  
│   ├── core/  
│   │   ├── pixel_canvas.h/.cpp             # 像素画布核心  
│   │   ├── drawing_tool.h/.cpp             # 绘图工具基类  
│   │   ├── brush_tool.h/.cpp               # 画笔工具  
│   │   ├── eraser_tool.h/.cpp              # 橡皮擦工具  
│   │   ├── selection_tool.h/.cpp           # 选择工具  
│   │   └── command_system.h/.cpp           # 命令系统(撤销/重做)  
│   ├── network/  
│   │   ├── network_manager.h/.cpp          # 网络管理器  
│   │   ├── protocol_handler.h/.cpp         # 协议处理器  
│   │   └── sync_manager.h/.cpp             # 同步管理器  
│   └── utils/  
│       ├── image_utils.h/.cpp              # 图像工具类  
│       └── math_utils.h/.cpp               # 数学工具类  
├── resources/  
│   ├── icons/                              # 图标资源  
│   ├── cursors/                            # 光标资源  
│   └── resources.qrc                       # 资源文件  
└── config/  
    └── app_config.h/.cpp                   # 应用配置

## 涉及的Qt类

1. UI相关  
QMainWindow: 主窗口基类  
QWidget: 自定义画布组件基类  
QToolBar: 工具栏  
QMenuBar: 菜单栏  
QStatusBar: 状态栏  
QDockWidget: 可停靠面板  
QScrollArea: 滚动区域  
QSplitter: 分割器  
2. 绘图相关  
QImage: 存储像素数据  
QPainter: 绘图操作  
QPaintEvent: 绘制事件  
QColor: 颜色管理  
QBrush: 画刷  
QPen: 画笔  
QCursor: 光标管理
3. 事件处理  
QMouseEvent: 鼠标事件  
QWheelEvent: 滚轮事件  
QKeyEvent: 键盘事件  
QPaintEvent: 绘制事件  
QResizeEvent: 窗口大小改变事件  
4. 网络通信  
QTcpSocket: TCP客户端  
QTcpServer: TCP服务器  
QNetworkAccessManager: 网络访问管理  
QJsonDocument: JSON文档处理  
QJsonObject: JSON对象  
5. 数据结构  
QStack: 栈容器(撤销/重做)  
QQueue: 队列容器(操作缓存)  
QList: 列表容器  
QMap: 映射容器  
QTimer: 定时器  
6. 文件操作  
QFileDialog: 文件对话框  
QImageReader: 图像读取  
QImageWriter: 图像写入  
QSettings: 配置文件管理
