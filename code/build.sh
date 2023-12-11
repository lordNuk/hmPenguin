#!/bin/bash

mkdir -p ../../build
pushd ../../build
c++ -g -o handmade ../hmPenguin/code/handmade.cpp `sdl2-config --cflags --libs`
popd
