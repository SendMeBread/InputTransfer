rmdir /s /q build

mkdir build
cd build

cmake -G Ninja ../src/win
cmake --build .

cd ../
rmdir /s /q build