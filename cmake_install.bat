rmdir /s /q build

mkdir build
cd build

cmake ../src/win
make

cd ../
rmdir /s /q build