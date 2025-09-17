cmake_minimum_required(VERSION 3.16)
project(PPainter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

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

add_executable(PPainter ${SOURCES} ${HEADERS})

target_link_libraries(PPainter Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network)

# 设置输出目录
set_target_properties(PPainter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
