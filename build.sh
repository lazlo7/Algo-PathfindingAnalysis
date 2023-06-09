#!/bin/sh

set -xeu

CC="c++"
CFLAGS="-Wall -Wextra -Wpedantic -std=c++2b -O3"
SRC_DIR="src/pathfinder"
SOURCES="${SRC_DIR}/main.cpp"
OUTPUT="pathfinder"
OUTPUT_DIR="./build"

mkdir -p $OUTPUT_DIR
$CC $CFLAGS -o $OUTPUT_DIR/$OUTPUT $SOURCES