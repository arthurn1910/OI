#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/bin
rm -f *
qmake -makefile ../src/FaceFeatures.pro
make
mv FaceFeatures solution3
