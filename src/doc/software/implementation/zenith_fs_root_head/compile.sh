#!bin/bash

echo "Compiling..."
gcc -c -g include/zenith.h -o bin/zenith.o
gcc -c -g main.c -o bin/main.o
gcc -o TestZenith.a bin/*
echo "Finished!"

