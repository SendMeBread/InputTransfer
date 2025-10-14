cmake_minimum_required(VERSION 4.1.2)

set(CMAKE_CXX_COMPILER "/msys64/mingw64/bin/g++")
set(CMAKE_C_COMPILER "/msys64/mingw64/bin/gcc")

project(InputTransfer CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/net_test")
add_executable(host_test ../net_test/win/host_test.cpp)
target_link_libraries(host_test ws2_32)
add_executable(client_test ../net_test/win/client_test.cpp)
target_link_libraries(client_test ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/mouse")
add_executable(client_cursor ../clients/singular/win/mouse/cursor.cpp)
target_link_libraries(client_cursor ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/keyboard/alphanumeric")
add_executable(client_w ../clients/singular/win/keyboard/alphanumeric/w.cpp)
target_link_libraries(client_w ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts")
add_executable(host_master ../hosts/win/master.cpp)
target_link_libraries(host_master shell32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/mouse")
add_executable(host_cursor ../hosts/win/mouse/cursor.cpp)
target_link_libraries(host_cursor ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/keyboard/alphanumeric")
add_executable(host_w ../hosts/win/keyboard/alphanumeric/w.cpp)
target_link_libraries(host_w ws2_32)