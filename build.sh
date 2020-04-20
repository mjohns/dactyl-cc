#!/bin/bash

if [ ! -d out ]; then
	mkdir out
fi
cd out

if [ "$1" == "--clean" ]; then
  echo "Clean"
  rm *.o
fi

if [ ! -f key.o ]; then
	echo "Building util"
	g++ -c -std=c++17 ../util/*.cc -I.. -I../util
	if [ $? -ne 0 ]; then
		echo "Failed to build util"
		exit 1
	fi
fi

echo "Building"
g++ -std=c++17 ../dactyl.cc ../key_data.cc *.o -I.. -I../util -o dactyl
if [ $? -ne 0 ]; then
  echo "Failed to build"
  exit 1
fi
echo "Running"
./dactyl && echo "Done"
