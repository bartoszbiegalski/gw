#!/bin/bash
set -e

echo "Buduję pod Windows (cross-kompilacja MXE)..."

export PATH="$HOME/mxe/usr/bin:$PATH"

MXE_PATH="$HOME/mxe"
TARGET="x86_64-w64-mingw32.static"
BUILD_DIR="build-windows"

# Dodaj ten export:
export PKG_CONFIG_PATH="$MXE_PATH/usr/$TARGET/lib/pkgconfig"

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

$MXE_PATH/usr/bin/${TARGET}-cmake .. -DCMAKE_BUILD_TYPE=Release

cmake --build . -- -j$(nproc)
