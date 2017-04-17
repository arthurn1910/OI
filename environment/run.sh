#!/bin/bash

#REQ:
#	bc http://gnuwin32.sourceforge.net/packages/bc.htm

BASIC=0

TEST_DATA_PATH="./data/test"
#TEST_IMAGES_PATH="${TRAIN_DATA_PATH}/images"
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
		RESULT_ANNOTATIONS_PATH="${RESULT_ANNOTATIONS_PATH}/${SOLUTION_NAME}"
		mkdir -p $RESULT_ANNOTATIONS_PATH
		;;
	esac
	shift # past argument or value
done


TEST_ANNOTATIONS_COUNT=$(ls -1 $TEST_ANNOTATIONS_PATH | wc -l)
RESULT_ANNOTATIONS_PATH="$RESULT_ANNOTATIONS_PATH/$(ls -t $RESULT_ANNOTATIONS_PATH | head -1)"
RESULT_ANNOTATIONS_COUNT=$(ls -1 $RESULT_ANNOTATIONS_PATH | wc -l)

if [[ $TEST_ANNOTATIONS_COUNT -ne $RESULT_ANNOTATIONS_COUNT ]]; then
	echo "error: annotations count does not match."
fi

FILES_NAME="$(ls -v $TEST_ANNOTATIONS_PATH)"

E:/!Google/!!POLI/Stopien_II/Sem_1/OI/build-OITester-Desktop_Qt_5_8_0_MinGW_32bit-Debug/debug/OITester.exe $BASIC $TEST_ANNOTATIONS_PATH $RESULT_ANNOTATIONS_PATH $TEST_ANNOTATIONS_COUNT $FILES_NAME 2> outfile.o
