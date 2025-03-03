#!/bin/sh

set -e

mkdir -p build
make build

XEPHYR=$(command -v Xephyr) # Absolute path of Xephyr's bin
xinit ./.xinitrc -- \
    "$XEPHYR" \
        :100 \
        -ac \
        -screen 1380x720\
        -host-cursor


