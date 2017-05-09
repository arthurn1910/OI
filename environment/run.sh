#!/bin/bash
set -e

BASIC=1
JAVA=0
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TEST_DATA_PATH="${DIR}/data/test"
TEST_IMAGE_PATH="${TEST_DATA_PATH}/images"
TEST_ANNOTATIONS_PATH="${TEST_DATA_PATH}/annotations"

RESULT_ANNOTATIONS_PATH="${DIR}/data/result"
    
while [[ $# -gt 0 ]]
do
    case $1 in
        --basic)
            BASIC=0
        ;;
        --extended)
            BASIC=1
        ;;
        --java)
            JAVA=1
        ;;
        *)
            SOLUTION_PATH=$1
            SOLUTION_NAME=$(basename $SOLUTION_PATH)
            RESULT_NEW_DIR=$(date +%F_%H-%M-%S)
            RESULT_ANNOTATIONS_PATH="${RESULT_ANNOTATIONS_PATH}/${SOLUTION_NAME}/$RESULT_NEW_DIR"
            mkdir -p $RESULT_ANNOTATIONS_PATH
        ;;
    esac
    shift # past argument or value
done

# Run solution
echo "Running ${SOLUTION_NAME} on testing set"
if [[ JAVA -eq 1 ]]
then
    # JAVA
    echo "Java"
else
    # C++
    $SOLUTION_PATH/bin/$SOLUTION_NAME $BASIC $TEST_IMAGE_PATH $RESULT_ANNOTATIONS_PATH
fi

# Test solution
echo "Running OITester to verify"
$DIR/bin/OITester $BASIC $TEST_ANNOTATIONS_PATH $RESULT_ANNOTATIONS_PATH
