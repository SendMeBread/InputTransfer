cmake_minimum_required(VERSION 4.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(InputTransfer)

link_libraries(X11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/net_test")
add_executable(host_test ../net_test/linux/host_test.cpp)
add_executable(client_test ../net_test/linux/client_test.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/mouse")
add_executable(client_cursor ../clients/singular/linux/mouse/cursor.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/mouse")
add_executable(host_cursor ../hosts/linux/mouse/cursor.cpp)