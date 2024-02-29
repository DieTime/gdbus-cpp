#!/bin/bash

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")
PROJECT_ROOT=$(dirname "$SCRIPT_ROOT")
SOURCES_ROOT=$PROJECT_ROOT/gdbus-c++

find "$SOURCES_ROOT" -iname '*.hpp' -o -name '*.cpp' -exec clang-format-15 --dry-run -Werror {} +;
