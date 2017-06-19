#!/bin/bash
set -e

export OI_SOLUTION=CPP

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

cp bin/solution2 ./deploy
