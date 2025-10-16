#!/bin/bash
set +x

if [ [ -d "build"] ]; then
    rm -rf build
fi
mkdir build
cd build

cmake ../src/linux
make

cd ../
rm -rf build