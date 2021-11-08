#!/usr/bin/env bash

echo "Building"
g++ -std=c++17 ../src/*.cc ../src/util/*.cc -I../src -I../src/util -o dactyl
if [ $? -ne 0 ]; then
  echo "Failed to build"
  exit 1
fi
echo "Running"
./dactyl && echo "Done"
