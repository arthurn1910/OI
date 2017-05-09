#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/bin
rm -f *
qmake -makefile ../src/solution3.pro
make
