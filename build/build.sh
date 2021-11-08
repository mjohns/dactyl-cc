#!/usr/bin/env bash

if [[ ! -d CMakeFiles ]]
then
  cmake ../src
fi

make
./dactyl
