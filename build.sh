#!/usr/bin/env bash
#!/usr/bin/env cmake

rm -rf build/
cmake -S . -B build
cmake --build build/
./build/Radium
