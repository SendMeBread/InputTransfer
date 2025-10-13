cmake_minimum_required(VERSION 4.1.2)
project(InputTransfer CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/win/net_test")

add_executable(host_test_win ../net_test/win/host_test.cpp)
target_link_libraries(host_test_win ws2_32)

add_executable(client_test_win ../net_test/win/client_test.cpp)
target_link_libraries(client_test_win ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/win/clients/singular/mouse")

add_executable(h_cursor_win ../hosts/win/mouse/cursor.cpp)
target_link_libraries(h_cursor_win ws2_32)

add_executable(c_cursor_win ../clients/singular/win/mouse/cursor.cpp)
target_link_libraries(c_cursor_win ws2_32)