cmake_minimum_required(VERSION 4.1.2)
project(InputTransfer CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/win/net_test")

add_executable(host_test ../net_test/win/host_test.cpp)
target_link_libraries(host_test ws2_32)

add_executable(client_test ../net_test/win/client_test.cpp)
target_link_libraries(client_test ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/win/clients/singular/mouse")

add_executable(client_cursor ../clients/singular/win/mouse/cursor.cpp)
target_link_libraries(client_cursor ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/win/hosts/mouse")

add_executable(host_cursor ../hosts/win/mouse/cursor.cpp)
target_link_libraries(host_cursor ws2_32)