# PPainter Project

## 工程结构

PixelWhiteboard/
├── CMakeLists.txt
├── main.cpp
├── src/
│   ├── ui/
│   │   ├── main_window.h/.cpp           # 主窗口
│   │   ├── canvas_widget.h/.cpp         # 画布组件
│   │   ├── tool_bar.h/.cpp             # 工具栏
│   │   ├── color_palette.h/.cpp        # 调色板
│   │   └── layer_panel.h/.cpp          # 图层面板
│   ├── core/
│   │   ├── pixel_canvas.h/.cpp         # 像素画布核心
│   │   ├── drawing_tool.h/.cpp         # 绘图工具基类
│   │   ├── brush_tool.h/.cpp           # 画笔工具
│   │   ├── eraser_tool.h/.cpp          # 橡皮擦工具
│   │   ├── selection_tool.h/.cpp       # 选择工具
│   │   └── command_system.h/.cpp       # 命令系统(撤销/重做)
│   ├── network/
│   │   ├── network_manager.h/.cpp      # 网络管理器
│   │   ├── protocol_handler.h/.cpp     # 协议处理器
│   │   └── sync_manager.h/.cpp         # 同步管理器
│   └── utils/
│       ├── image_utils.h/.cpp          # 图像工具类
│       └── math_utils.h/.cpp           # 数学工具类
├── resources/
│   ├── icons/                          # 图标资源
│   ├── cursors/                        # 光标资源
│   └── resources.qrc                   # 资源文件
└── config/
    └── app_config.h/.cpp               # 应用配置
