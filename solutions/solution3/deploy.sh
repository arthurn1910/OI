#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

export OI_SOLUTION CPP

cp bin/NeuralNetwork ./deploy/solution3
