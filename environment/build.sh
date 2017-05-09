#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/bin
rm -f *
qmake -makefile ../src/OITester.pro
make
