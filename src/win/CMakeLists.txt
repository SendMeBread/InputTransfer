cmake_minimum_required(VERSION 4.1.0)

set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")


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
add_executable(client_f ../clients/singular/win/keyboard/alphanumeric/f.cpp)
add_executable(client_g ../clients/singular/win/keyboard/alphanumeric/g.cpp)
add_executable(client_h ../clients/singular/win/keyboard/alphanumeric/h.cpp)
add_executable(client_i ../clients/singular/win/keyboard/alphanumeric/i.cpp)
add_executable(client_j ../clients/singular/win/keyboard/alphanumeric/j.cpp)
add_executable(client_k ../clients/singular/win/keyboard/alphanumeric/k.cpp)
add_executable(client_l ../clients/singular/win/keyboard/alphanumeric/l.cpp)
add_executable(client_m ../clients/singular/win/keyboard/alphanumeric/m.cpp)
add_executable(client_n ../clients/singular/win/keyboard/alphanumeric/n.cpp)
add_executable(client_o ../clients/singular/win/keyboard/alphanumeric/o.cpp)
add_executable(client_p ../clients/singular/win/keyboard/alphanumeric/p.cpp)
add_executable(client_q ../clients/singular/win/keyboard/alphanumeric/q.cpp)
add_executable(client_r ../clients/singular/win/keyboard/alphanumeric/r.cpp)
add_executable(client_s ../clients/singular/win/keyboard/alphanumeric/s.cpp)
add_executable(client_t ../clients/singular/win/keyboard/alphanumeric/t.cpp)
add_executable(client_u ../clients/singular/win/keyboard/alphanumeric/u.cpp)
add_executable(client_v ../clients/singular/win/keyboard/alphanumeric/v.cpp)
add_executable(client_w ../clients/singular/win/keyboard/alphanumeric/w.cpp)
add_executable(client_x ../clients/singular/win/keyboard/alphanumeric/x.cpp)
add_executable(client_y ../clients/singular/win/keyboard/alphanumeric/y.cpp)
add_executable(client_z ../clients/singular/win/keyboard/alphanumeric/z.cpp)

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

add_executable(host_a ../hosts/win/singular/keyboard/alphanumeric/a.cpp)
add_executable(host_b ../hosts/win/singular/keyboard/alphanumeric/b.cpp)
add_executable(host_c ../hosts/win/singular/keyboard/alphanumeric/c.cpp)
add_executable(host_e ../hosts/win/singular/keyboard/alphanumeric/e.cpp)
add_executable(host_f ../hosts/win/singular/keyboard/alphanumeric/f.cpp)
add_executable(host_w ../hosts/win/singular/keyboard/alphanumeric/w.cpp)