cmake_minimum_required(VERSION 3.16)
project(PPainter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件列表
set(SOURCES
    main.cpp
    src/MainWindow.cpp
    src/DrawingArea.cpp
    src/DrawingTool.cpp
    src/Imagepaster.cpp
)

set(HEADERS
    src/MainWindow.h
    src/DrawingArea.h
    src/DrawingTool.h
    src/Imagepaster.h
)

# 添加 Qt 资源
qt_add_resources(RES_SRCS resources.qrc)

add_executable(PPainter ${SOURCES} ${HEADERS} ${RES_SRCS})

target_link_libraries(PPainter Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network)

set_target_properties(PPainter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
