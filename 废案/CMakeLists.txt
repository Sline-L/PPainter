cmake_minimum_required(VERSION 3.16)
project(PPainter v0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network Concurrent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 预编译头文件
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)


# 源文件分组
set(UI_SOURCES
    src/ui/main_window.cpp
    src/ui/canvas_widget.cpp
    src/ui/tool_bar.cpp
    src/ui/color_palette.cpp
    src/ui/layer_panel.cpp
)
set(CORE_SOURCES
    src/core/pixel_canvas.cpp
    src/core/drawing_tool.cpp
    src/core/brush_tool.cpp
    src/core/eraser_tool.cpp
    src/core/selection_tool.cpp
    src/core/command_system.cpp
)
set(NETWORK_SOURCES
    src/network/network_manager.cpp
    src/network/protocol_handler.cpp
    src/network/sync_manager.cpp
)
set(UTILS_SOURCES
    src/utils/image_utils.cpp
    src/utils/math_utils.cpp
)
set(CONFIG_SOURCES
    config/app_config.cpp
)
# 头文件分组
set(UI_HEADERS
    src/ui/main_window.h
    src/ui/canvas_widget.h
    src/ui/tool_bar.h
    src/ui/color_palette.h
    src/ui/layer_panel.h
)
set(CORE_HEADERS
    src/core/pixel_canvas.h
    src/core/drawing_tool.h
    src/core/brush_tool.h
    src/core/eraser_tool.h
    src/core/selection_tool.h
    src/core/command_system.h
)
set(NETWORK_HEADERS
    src/network/network_manager.h
    src/network/protocol_handler.h
    src/network/sync_manager.h
)
set(UTILS_HEADERS
    src/utils/image_utils.h
    src/utils/math_utils.h
)
set(CONFIG_HEADERS
    config/app_config.h
)

# add_executable(ElectronicWhiteboard ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME}
    main.cpp
    ${UI_SOURCES} ${UI_HEADERS}
    ${CORE_SOURCES} ${CORE_HEADERS}
    ${NETWORK_SOURCES} ${NETWORK_HEADERS}
    ${UTILS_SOURCES} ${UTILS_HEADERS}
    ${CONFIG_SOURCES} ${CONFIG_HEADERS}
    resources/resources.qrc
)

# target_link_libraries(ElectronicWhiteboard Qt6::Core Qt6::Widgets Qt6::Gui)
target_link_libraries(${PROJECT_NAME}
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

set_target_properties(ElectronicWhiteboard PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
