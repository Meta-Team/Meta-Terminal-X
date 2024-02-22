#!/bin/bash
WINSRCDIR=$(realpath $(dirname $0)/..)
echo $WINSRCDIR
cmake -GNinja -B $WINSRCDIR/build -DCMAKE_PREFIX_PATH=~/Qt/6.5.3/macos $WINSRCDIR 
