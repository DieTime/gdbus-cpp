#!/bin/bash

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")
PROJECT_ROOT=$(dirname "$SCRIPT_ROOT")
SOURCES_ROOT="$PROJECT_ROOT/gdbus-c++"
EXAMPLE_ROOT="$PROJECT_ROOT/example"
BUILD_ROOT="$PROJECT_ROOT/builddir"

if [ -d "$BUILD_ROOT" ]; then
    rm -rf "$BUILD_ROOT"
fi

meson setup \
    -DGDBUS_CPP_BUILD_EXAMPLE=true \
    -DGDBUS_CPP_ENABLE_DEBUG_LOGGING=true \
    "$BUILD_ROOT" \
    "$PROJECT_ROOT"

clang-tidy-15 -p "$BUILD_ROOT" $(find "$SOURCES_ROOT" "$EXAMPLE_ROOT" -regex '.+\.[hc]pp')
