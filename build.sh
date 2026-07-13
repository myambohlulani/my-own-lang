#!/usr/bin/env bash
#!/usr/bin/env cmake

rm -rf build/
cmake -S . -B build -G Ninja
cmake --build build/

