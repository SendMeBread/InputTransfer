cmake_minimum_required(VERSION 4.1.0)

set(CMAKE_CXX_COMPILER "/msys64/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "/msys64/mingw64/bin/gcc.exe")

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
add_executable(client_a ../clients/singular/win/keyboard/alphanumeric/a.cpp)
target_link_libraries(client_a ws2_32)

add_executable(client_b ../clients/singular/win/keyboard/alphanumeric/b.cpp)
target_link_libraries(client_b ws2_32)

add_executable(client_c ../clients/singular/win/keyboard/alphanumeric/c.cpp)
target_link_libraries(client_c ws2_32)

add_executable(client_e ../clients/singular/win/keyboard/alphanumeric/e.cpp)
target_link_libraries(client_e ws2_32)

add_executable(client_w ../clients/singular/win/keyboard/alphanumeric/w.cpp)
target_link_libraries(client_w ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts")
add_executable(host_master ../hosts/win/master.cpp)
target_link_libraries(host_master ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/mouse")
add_executable(host_cursor ../hosts/win/mouse/cursor.cpp)
target_link_libraries(host_cursor ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/keyboard/alphanumeric")
add_executable(host_w ../hosts/win/keyboard/alphanumeric/w.cpp)
target_link_libraries(host_w ws2_32)