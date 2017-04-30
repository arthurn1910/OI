#!/bin/bash

BASIC=1
TEST_DATA_PATH="./data/test"
TEST_IMAGE_PATH="${TEST_DATA_PATH}/images"
TEST_ANNOTATIONS_PATH="${TEST_DATA_PATH}/annotations"

RESULT_ANNOTATIONS_PATH="./data/result"
	
while [[ $# -gt 0 ]]
do
	case $1 in
		--basic)
			BASIC=0
		;;
		--extended)
			BASIC=1
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

#RESULT_ANNOTATIONS_PATH="${RESULT_ANNOTATIONS_PATH}/annotations"

# Run solution
$SOLUTION_PATH/bin/$SOLUTION_NAME $BASIC $TEST_IMAGE_PATH $RESULT_ANNOTATIONS_PATH

# Test solution
./bin/OITester $BASIC $TEST_ANNOTATIONS_PATH $RESULT_ANNOTATIONS_PATH
