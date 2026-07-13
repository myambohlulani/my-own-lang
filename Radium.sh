#!/usr/bin/env bash

rm -f ./tmp/out*
./build/radium "$1"
./bin/app
