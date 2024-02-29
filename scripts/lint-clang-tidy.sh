#!/bin/bash

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")
PROJECT_ROOT=$(dirname "$SCRIPT_ROOT")
SOURCES_ROOT=$PROJECT_ROOT/gdbus-c++
BUILD_ROOT="$PROJECT_ROOT"/builddir

meson setup --reconfigure "$BUILD_ROOT" "$PROJECT_ROOT"

clang-tidy-15 -p "$BUILD_ROOT" "$SOURCES_ROOT"/*.cpp "$SOURCES_ROOT"/*.hpp
