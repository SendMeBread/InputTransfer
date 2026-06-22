@echo off
IF exist build rmdir /s /q build
mkdir build
cd build

cmake -G Ninja -S ../src/win
cmake --build .

cd ../
rmdir /s /q build