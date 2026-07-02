#!/usr/bin/env bash

rm -rf ./bin
./build.sh
./build/radium tests/main.hlx

# compiling and linking
nasm -f elf64 ./bin/out.asm -o ./bin/out.o
ld -o ./bin/out ./bin/out.o

# running the file
./bin/out

echo $?
