#!/bin/bash

rm -rf build

mkdir build
cd build

cmake ../src/linux
make

cd ../
rm -rf build