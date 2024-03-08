#!/bin/bash

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")
PROJECT_ROOT=$(dirname "$SCRIPT_ROOT")
SOURCES_ROOT="$PROJECT_ROOT/gdbus-c++"
EXAMPLE_ROOT="$PROJECT_ROOT/example"

find "$SOURCES_ROOT" "$EXAMPLE_ROOT" \
    -regex '.+\.[hc]pp' \
    -exec clang-format-15 --dry-run -Werror {} +;
