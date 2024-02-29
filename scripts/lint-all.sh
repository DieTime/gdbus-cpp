#!/bin/bash

set -e

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_ROOT=$(dirname "$SCRIPT_PATH")

source "$SCRIPT_ROOT"/lint-clang-format.sh
source "$SCRIPT_ROOT"/lint-clang-tidy.sh
source "$SCRIPT_ROOT"/lint-github-actions.sh
