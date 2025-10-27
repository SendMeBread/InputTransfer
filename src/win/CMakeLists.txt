cmake_minimum_required(VERSION 4.1.0)

set(CMAKE_CXX_COMPILER "/msys64/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "/msys64/mingw64/bin/gcc.exe")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(InputTransfer)

link_libraries(ws2_32)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/net_test")
add_executable(host_test ../net_test/win/host_test.cpp)
add_executable(client_test ../net_test/win/client_test.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/mouse")
add_executable(client_cursor ../clients/singular/win/mouse/cursor.cpp)

target_link_libraries(client_cursor INTERFACE /sendMouseData.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/keyboard/alphanumeric")

add_executable(client_a ../clients/singular/win/keyboard/alphanumeric/a.cpp)
add_executable(client_b ../clients/singular/win/keyboard/alphanumeric/b.cpp)
add_executable(client_c ../clients/singular/win/keyboard/alphanumeric/c.cpp)
add_executable(client_d ../clients/singular/win/keyboard/alphanumeric/d.cpp)
add_executable(client_e ../clients/singular/win/keyboard/alphanumeric/e.cpp)

add_executable(client_w ../clients/singular/win/keyboard/alphanumeric/w.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/clients/singular/keyboard/functional")
add_executable(client_caps ../clients/singular/win/keyboard/functional/caps.cpp)
add_executable(client_delete ../clients/singular/win/keyboard/functional/delete.cpp)
add_executable(client_shift ../clients/singular/win/keyboard/functional/shift.cpp)
add_executable(client_left ../clients/singular/win/keyboard/functional/left.cpp)
add_executable(client_right ../clients/singular/win/keyboard/functional/right.cpp)
add_executable(client_up ../clients/singular/win/keyboard/functional/up.cpp)
add_executable(client_down ../clients/singular/win/keyboard/functional/down.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts")
add_executable(host_master ../hosts/win/master.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/singular/mouse")
add_executable(host_cursor ../hosts/win/singular/mouse/cursor.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/hosts/singular/keyboard/alphanumeric")
add_executable(host_w ../hosts/win/singular/keyboard/alphanumeric/w.cpp)