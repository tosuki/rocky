#!/bin/bash
set -e
mkdir -p build
make build

XEPHYR=$(command -v Xephyr)
xinit ./.xinitrc -- \
    "$XEPHYR" \
    -ac \
    -screen 1380x720 \
    -host-cursor

