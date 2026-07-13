#!/usr/bin/env bash
set -uo pipefail # running from root repo

PASS=0
FAIL=0
TEST_DIR="tests/"
TEST_DIR_PRINTF="tests/printf"
BUILD_BIN="./build/radium"

test_case() {
    local testCase="$1"
    local name
    local extension="$2"

    name=$(basename "$testCase" "$extension")
    local expected_file="${TEST_DIR}/${name}.expected"

    # checking for correct file
    if [  ! -f "$expected_file"  ]; then
        echo "SKIP: $name (there is no .expected file found here)"
        return
    fi

    local expected_exit
    expected_exit=$(cat "$expected_file")

    if ! "$BUILD_BIN" "$testCase" -o "./bin/${name}.asm"; then
        echo "FAIL: $name (compilation failed)"
        FAIL=$((FAIL + 1))
        return
    fi

    # compilation of files
    if ! mips-linux-gnu-as "./bin/${name}.asm" -o "./bin/${name}.o"; then
        echo "FAIL: $name (compilation failed - qemu)"
        FAIL=$((FAIL + 1))
        return
    fi

    # liking of files
    if ! mips-linux-gnu-ld "./bin/${name}.o" -o "./bin/${name}"; then
       echo "FAIL: $name (linking failed)"
       FAIL=$((FAIL + 1))
       return
    fi

    # running
    qemu-mips "./bin/${name}"
    local actual_exit=$?

    ## checking the output
    if [ "$actual_exit" -eq "$expected_exit" ]; then
        echo "PASS: $name"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $name (expected $expected_exit, got $actual_exit)"
        FAIL=$((FAIL + 1))
    fi

}

# testing those in tests/ folder
for testcase in "${TEST_DIR}"/*.hlx; do
     [ -e "$testcase" ] || continue
    test_case "$testcase" ".hlx"
done

# testing printf
for testcase in "${TEST_DIR_PRINTF}"/*.hlx; do
    [ -e "$testcase" ] || continue
    test_case "$testcase" ".hlx"
done

echo "Results: $PASS passed, $FAIL failed"

[ "$FAIL" -eq 0 ] || exit 1
