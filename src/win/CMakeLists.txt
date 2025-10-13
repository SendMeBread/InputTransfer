cmake_minimum_required(VERSION 4.1.2)
project(InputTransfer CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

add_executable(h_cursor_win ../hosts/win/mouse/cursor.cpp)
target_link_libraries(h_cursor_win ws2_32)

add_executable(c_cursor_win ../clients/singular/win/mouse/cursor.cpp)
target_link_libraries(c_cursor_win ws2_32)