#!/usr/bin/env bash

./build/radium ./tests/main.hlx
mips-linux-gnu-as ./bin/out.asm -o ./bin/out.o
mips-linux-gnu-ld ./bin/out.o -o ./bin/app

qemu-mips ./bin/app
echo $?
