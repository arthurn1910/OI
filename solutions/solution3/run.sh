#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -f $DIR/deploy/solution3 ]; then
    $DIR/deploy/solution3 1 /home/tajgeer/Repozytoria/ObliczeniaInteligentne/environment/data/test/images /home/tajgeer/Repozytoria/NeuralNetwork/results
fi
