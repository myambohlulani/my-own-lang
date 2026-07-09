#!/usr/bin/env bash

./build/radium ./tests/main.hlx
nasm -f elf64 ./bin/out.asm -o ./bin/out.o
ld ./bin/out.o -o ./bin/app

./bin/app
echo $?
