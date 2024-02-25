#!bin/bash

echo "Compiling..."
clang -c -g include/zenith.h -o bin/zenith.o
clang -c -g main.c -o bin/main.o
clang -o TestZenith.a bin/*
echo "Finished!"

