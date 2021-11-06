cmake_minimum_required(VERSION 3.1)

project(terminal-OS)

set(CMAKE_CXX_STANDARD 17)

add_executable(terminal-OS main.cpp
                           operating_system/operating_system.cpp
                           operating_system/file_system.cpp
                           operating_system/window.cpp
                           operating_system/bordered_window.cpp
                           operating_system/taskbar.cpp
                           operating_system/icon.cpp

                           applications/terminal.cpp
                           applications/paint.cpp

                           util/mouse_event.cpp
                           util/rect.cpp
                           util/size.cpp
                           util/coordinates.cpp
                           util/image.cpp)

target_include_directories(terminal-OS PRIVATE operating_system applications util)
