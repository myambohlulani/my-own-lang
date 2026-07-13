#!/usr/bin/env bash

rm -f ./tmp/out*
./build/radium "$1"
nasm -f elf64 ./bin/out.s -o ./bin/app.o
ld ./bin/app.o -o ./bin/app

./bin/app
