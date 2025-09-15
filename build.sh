#!/bin/sh
# Build script for SDL2 project

g++ main.cpp -o sdl2_app $(sdl2-config --cflags --libs)
