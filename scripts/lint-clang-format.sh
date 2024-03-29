#!/bin/bash

# SPDX-FileCopyrightText: Copyright 2024 Denis Glazkov <glazzk.off@mail.ru>
# SPDX-License-Identifier: Apache-2.0

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")
PROJECT_ROOT=$(dirname "$SCRIPT_ROOT")
SOURCES_ROOT="$PROJECT_ROOT/gdbus-c++"
SAMPLES_ROOT="$PROJECT_ROOT/samples"

find "$SOURCES_ROOT" "$SAMPLES_ROOT" \
    -regex '.+\.[hc]pp' \
    -exec clang-format-15 --dry-run -Werror {} +;
