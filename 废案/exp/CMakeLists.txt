cmake_minimum_required(VERSION 3.16)
project(PixelWhiteboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network Concurrent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

set(SOURCES
    main.cpp
    src/ui/main_window.cpp
    src/ui/canvas_widget.cpp
    src/ui/tool_bar.cpp
    src/ui/color_palette.cpp
    src/ui/layer_panel.cpp
    src/core/pixel_canvas.cpp
    src/core/drawing_tool.cpp
    src/core/brush_tool.cpp
    src/core/eraser_tool.cpp
    src/core/selection_tool.cpp
    src/core/command_system.cpp
    src/network/network_manager.cpp
    src/network/protocol_handler.cpp
    src/network/sync_manager.cpp
    src/utils/image_utils.cpp
    src/utils/math_utils.cpp
    config/app_config.cpp
)

set(HEADERS
    src/ui/main_window.h
    src/ui/canvas_widget.h
    src/ui/tool_bar.h
    src/ui/color_palette.h
    src/ui/layer_panel.h
    src/core/pixel_canvas.h
    src/core/drawing_tool.h
    src/core/brush_tool.h
    src/core/eraser_tool.h
    src/core/selection_tool.h
    src/core/command_system.h
    src/network/network_manager.h
    src/network/protocol_handler.h
    src/network/sync_manager.h
    src/utils/image_utils.h
    src/utils/math_utils.h
    config/app_config.h
)

qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 如果有资源，使用 qt_add_resources
# qt_add_resources(${PROJECT_NAME} "resources"
#     PREFIX "/"
#     FILES
#         resources/resources.qrc
# )

# 统一使用 keyword 签名（PRIVATE）
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        Qt6::Network
        Qt6::Concurrent
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
