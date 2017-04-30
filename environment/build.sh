#!/bin/bash

cd bin
rm -f *
qmake -makefile ../src/OITester.pro
make
