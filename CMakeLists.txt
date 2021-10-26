cmake_minimum_required(VERSION 3.1)

project(terminal-OS)

set(CMAKE_CXX_STANDARD 17)

add_executable(terminal-OS main.cpp
                           operating_system.cpp
                           window.cpp
                           bordered_window.cpp
                           mouse_event.cpp
                           rect.cpp
                           size.cpp
                           coordinates.cpp
                           image.cpp
                           icon.cpp)
