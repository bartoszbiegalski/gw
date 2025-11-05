#!/bin/bash
set -e

echo "Buduję pod Windows (cross-kompilacja MXE)..."

MXE_PATH="$HOME/mxe"
TARGET="x86_64-w64-mingw32.static"
BUILD_DIR="build-windows"

export PATH="$MXE_PATH/usr/bin:$PATH"

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Konfiguracja CMake za pomocą MXE wrapper
$MXE_PATH/usr/bin/${TARGET}-cmake .. -DCMAKE_BUILD_TYPE=Release

# Budowa
cmake --build . -- -j$(nproc)
