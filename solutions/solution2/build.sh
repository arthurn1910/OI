#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rm -f $DIR/bin
mvn package #-PotherOutputDir
make
