#!/bin/bash

#REQ:
#	bc http://gnuwin32.sourceforge.net/packages/bc.htm

BASIC=true
LEARING_DATA=60

#TRAIN_DATA_PATH="./data/train"
#TRAIN_IMAGES_PATH="${TRAIN_DATA_PATH}/images"
#TRAIN_ANNOTATIONS_PATH="${TRAIN_DATA_PATH}/annotations"

TEST_DATA_PATH="./data/test"
#TEST_IMAGES_PATH="${TRAIN_DATA_PATH}/images"
TEST_ANNOTATIONS_PATH="${TEST_DATA_PATH}/annotations"

RESULT_ANNOTATIONS_PATH="./data/result"
	
while [[ $# -gt 0 ]]
do
	case $1 in
		--basic)
		BASIC=true
		;;
		--extended)
		BASIC=false
		;;
		-c)
		LEARING_DATA=$2
		shift
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

re='^[0-9]+$'
if ! [[ $LEARING_DATA =~ $re ]]; then
   echo "error: Not a number" >&2; exit 1
fi

if [[ $LEARING_DATA -gt 100 ]]; then
   echo "error: Too big" >&2; exit 1
fi

# RUN PROGRAM

TEST_ANNOTATIONS_COUNT=$(ls -1 $TEST_ANNOTATIONS_PATH | wc -l)
RESULT_ANNOTATIONS_PATH="$RESULT_ANNOTATIONS_PATH/$(ls -t $RESULT_ANNOTATIONS_PATH | head -1)"
RESULT_ANNOTATIONS_COUNT=$(ls -1 $RESULT_ANNOTATIONS_PATH | wc -l)

if [[ $TEST_ANNOTATIONS_COUNT -ne $RESULT_ANNOTATIONS_COUNT ]]; then
	echo "error: annotations count does not match."
fi

FILES_NAME=$(ls -v $TEST_ANNOTATIONS_PATH)


PRECISION="0.15"
FILE_STATUS=()

RIGHT_EYE_PUPIL_DISTANCES=()
RIGHT_EYE_PUPIL_MEAN=0
RIGHT_EYE_PUPIL_ERROR=0

LEFT_EYE_PUPIL_DISTANCES=()
LEFT_EYE_PUPIL_MEAN=0
LEFT_EYE_PUPIL_ERROR=0

if ! [[ $BASIC ]]; then
	RIGHT_MOUTH_DISTANCES=()
	RIGHT_MOUTH_MEAN=0
	RIGHT_MOUTH_ERROR=0

	LEFT_MOUTH_DISTANCES=()
	LEFT_MOUTH_MEAN=0
	LEFT_MOUTH_ERROR=0

	RIGHT_OUTER_EYEBROW_DISTANCES=()
	RIGHT_OUTER_EYEBROW_MEAN=0
	RIGHT_OUTER_EYEBROW_ERROR=0

	RIGHT_INNER_EYEBROW_DISTANCES=()
	RIGHT_INNER_EYEBROW_MEAN=0
	RIGHT_INNER_EYEBROW_ERROR=0

	LEFT_INNER_EYEBROW_DISTANCES=()
	LEFT_INNER_EYEBROW_MEAN=0
	LEFT_INNER_EYEBROW_ERROR=0

	LEFT_OUTER_EYEBROW_DISTANCES=()
	LEFT_OUTER_EYEBROW_MEAN=0
	LEFT_OUTER_EYEBROW_ERROR=0

	RIGHT_TEMPLE_DISTANCES=()
	RIGHT_TEMPLE_MEAN=0
	RIGHT_TEMPLE_ERROR=0

	RIGHT_OUTER_EYE_DISTANCES=()
	RIGHT_OUTER_EYE_MEAN=0
	RIGHT_OUTER_EYE_ERROR=0

	RIGHT_INNER_EYE_DISTANCES=()
	RIGHT_INNER_EYE_MEAN=0
	RIGHT_INNER_EYE_ERROR=0

	LEFT_INNER_EYE_DISTANCES=()
	LEFT_INNER_EYE_MEAN=0
	LEFT_INNER_EYE_ERROR=0

	LEFT_OUTER_EYE_DISTANCES=()
	LEFT_OUTER_EYE_MEAN=0
	LEFT_OUTER_EYE_ERROR=0

	LEFT_TEMPLE_DISTANCES=()
	LEFT_TEMPLE_MEAN=0
	LEFT_TEMPLE_ERROR=0

	TIP_OF_NOSE_DISTANCES=()
	TIP_OF_NOSE_MEAN=0
	TIP_OF_NOSE_ERROR=0

	RIGHT_NOSTRIL_DISTANCES=()
	RIGHT_NOSTRIL_MEAN=0
	RIGHT_NOSTRIL_ERROR=0

	LEFT_NOSTRIL_DISTANCES=()
	LEFT_NOSTRIL_MEAN=0
	LEFT_NOSTRIL_ERROR=0

	CENTRE_POINT_OF_UPPER_LIP_DISTANCES=()
	CENTRE_POINT_OF_UPPER_LIP_MEAN=0
	CENTRE_POINT_OF_UPPER_LIP_ERROR=0

	CENTRE_POINT_OF_LOWER_LIP_DISTANCES=()
	CENTRE_POINT_OF_LOWER_LIP_MEAN=0
	CENTRE_POINT_OF_LOWER_LIP_ERROR=0

	TIP_OF_CHIN_DISTANCES=()
	TIP_OF_CHIN_MEAN=0
	TIP_OF_CHIN_ERROR=0
fi

for file in $FILES_NAME
do
	echo $file
	test_file_content=($(sed -n '/{/,/}/{/{/b;/}/b;p}' $TEST_ANNOTATIONS_PATH/$file))
	result_file_content=($(sed -n '/{/,/}/{/{/b;/}/b;p}' $RESULT_ANNOTATIONS_PATH/$file))
	#file_content=$(awk '/{/{flag=1;next}/}/{flag=0}flag' $TEST_ANNOTATIONS_PATH/$file)
	
	correct_location=true
	relative_distance=$(echo "sqrt((${test_file_content[0]}-${test_file_content[2]})^2 + (${test_file_content[1]}-${test_file_content[3]})^2)" | bc -l)
	
	tmp_right_eye_dist=$(echo "sqrt((${test_file_content[0]}-${result_file_content[0]})^2 + (${test_file_content[1]}-${result_file_content[1]})^2)/$relative_distance" | bc -l)

	tmp_left_eye_dist=$(echo "sqrt((${test_file_content[2]}-${result_file_content[2]})^2 + (${test_file_content[3]}-${result_file_content[3]})^2)/$relative_distance" | bc -l)
	
	if ! [[ $BASIC ]]; then
		relative_distance=$(echo "sqrt((${test_file_content[26]}-${test_file_content[38]})^2 + (${test_file_content[27]}-${test_file_content[39]})^2)*2" | bc -l)

		pointer=0 # right eye
		tmp_right_eye_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)

		pointer=$pointer+2 # left eye
		tmp_left_eye_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		
		pointer=$pointer+2 # right mouth 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_MOUTH_MEAN=$RIGHT_MOUTH_MEAN+tmp_dist
		RIGHT_MOUTH_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # left mouth 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_MOUTH_MEAN=$LEFT_MOUTH_MEAN+tmp_dist
		LEFT_MOUTH_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_OUTER_EYEBROW 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_OUTER_EYEBROW_MEAN=$RIGHT_OUTER_EYEBROW_MEAN+tmp_dist
		RIGHT_OUTER_EYEBROW_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_INNER_EYEBROW
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_INNER_EYEBROW_MEAN=$RIGHT_INNER_EYEBROW_MEAN+tmp_dist
		RIGHT_INNER_EYEBROW_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_INNER_EYEBROW
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_INNER_EYEBROW_MEAN=$LEFT_INNER_EYEBROW_MEAN+tmp_dist
		LEFT_INNER_EYEBROW_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_OUTER_EYEBROW
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_OUTER_EYEBROW_MEAN=$LEFT_OUTER_EYEBROW_MEAN+tmp_dist
		LEFT_OUTER_EYEBROW_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_TEMPLE 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_TEMPLE_MEAN=$RIGHT_TEMPLE_MEAN+tmp_dist
		RIGHT_TEMPLE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_OUTER_EYE
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_OUTER_EYE_MEAN=$RIGHT_OUTER_EYE_MEAN+tmp_dist
		RIGHT_OUTER_EYE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_INNER_EYE 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_INNER_EYE_MEAN=$RIGHT_INNER_EYE_MEAN+tmp_dist
		RIGHT_INNER_EYE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_INNER_EYE
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_INNER_EYE_MEAN=$LEFT_INNER_EYE_MEAN+tmp_dist
		LEFT_INNER_EYE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_OUTER_EYE
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_OUTER_EYE_MEAN=$LEFT_OUTER_EYE_MEAN+tmp_dist
		LEFT_OUTER_EYE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_TEMPLE
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_TEMPLE_MEAN=$LEFT_TEMPLE_MEAN+tmp_dist
		LEFT_TEMPLE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # TIP_OF_NOSE 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		TIP_OF_NOSE_MEAN=$TIP_OF_NOSE_MEAN+tmp_dist
		TIP_OF_NOSE_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # RIGHT_NOSTRIL
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		RIGHT_NOSTRIL_MEAN=$RIGHT_NOSTRIL_MEAN+tmp_dist
		RIGHT_NOSTRIL_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # LEFT_NOSTRIL
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		LEFT_NOSTRIL_MEAN=$LEFT_NOSTRIL_MEAN+tmp_dist
		LEFT_NOSTRIL_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # CENTRE_POINT_OF_UPPER_LIP 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		CENTRE_POINT_OF_UPPER_LIP_MEAN=$CENTRE_POINT_OF_UPPER_LIP_MEAN+tmp_dist
		CENTRE_POINT_OF_UPPER_LIP_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # CENTRE_POINT_OF_LOWER_LIP 
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		CENTRE_POINT_OF_LOWER_LIP_MEAN=$CENTRE_POINT_OF_LOWER_LIP_MEAN+tmp_dist
		CENTRE_POINT_OF_LOWER_LIP_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
		
		pointer=$pointer+2 # TIP_OF_CHIN
		tmp_dist=$(echo "sqrt((${test_file_content[$pointer]}-${result_file_content[$pointer]})^2 + (${test_file_content[$pointer+1]}-${result_file_content[$pointer+1]})^2)/$relative_distance" | bc -l)
		TIP_OF_CHIN_MEAN=$TIP_OF_CHIN_MEAN+tmp_dist
		TIP_OF_CHIN_DISTANCES+=$tmp_dist
		if [[ $(echo "$tmp_dist < $PRECISION" | bc) -eq 0 ]]; then
			correct_location=false
		fi
	fi
	
	RIGHT_EYE_PUPIL_DISTANCES+=$tmp_right_eye_dist
	RIGHT_EYE_PUPIL_MEAN=$RIGHT_EYE_PUPIL_MEAN+tmp_right_eye_dist
	if [[ $(echo "$tmp_right_eye_dist < $PRECISION" | bc) -eq 0 ]]; then
		correct_location=false
	fi
	
	LEFT_EYE_PUPIL_DISTANCES+=$tmp_left_eye_dist
	LEFT_EYE_PUPIL_MEAN=$RIGHT_EYE_PUPIL_MEAN+tmp_left_eye_dist
	if [[ $(echo "$tmp_left_eye_dist < $PRECISION" | bc) -eq 0 ]]; then
		correct_location=false
	fi
	echo $correct_location
	FILE_STATUS+=correct_location
done

RIGHT_EYE_PUPIL_MEAN=$(echo "$RIGHT_EYE_PUPIL_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
LEFT_EYE_PUPIL_MEAN=$(echo "$LEFT_EYE_PUPIL_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
if [[ BASIC ]]; then
	RIGHT_MOUTH_MEAN=$(echo "$RIGHT_MOUTH_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_MOUTH_MEAN=$(echo "$LEFT_MOUTH_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_OUTER_EYEBROW_MEAN=$(echo "$RIGHT_OUTER_EYEBROW_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_INNER_EYEBROW_MEAN=$(echo "$RIGHT_INNER_EYEBROW_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_INNER_EYEBROW_MEAN=$(echo "$LEFT_INNER_EYEBROW_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_OUTER_EYEBROW_MEAN=$(echo "$LEFT_OUTER_EYEBROW_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_TEMPLE_MEAN=$(echo "$RIGHT_TEMPLE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_OUTER_EYE_MEAN=$(echo "$RIGHT_OUTER_EYE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_INNER_EYE_MEAN=$(echo "$RIGHT_INNER_EYE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_INNER_EYE_MEAN=$(echo "$LEFT_INNER_EYE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_OUTER_EYE_MEAN=$(echo "$LEFT_OUTER_EYE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_TEMPLE_MEAN=$(echo "$LEFT_TEMPLE_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	RIGHT_NOSTRIL_MEAN=$(echo "$RIGHT_NOSTRIL_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	LEFT_NOSTRIL_MEAN=$(echo "$LEFT_NOSTRIL_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	CENTRE_POINT_OF_LOWER_LIP_MEAN=$(echo "$CENTRE_POINT_OF_LOWER_LIP_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
	TIP_OF_CHIN_MEAN=$(echo "$TIP_OF_CHIN_MEAN/$TEST_ANNOTATIONS_COUNT" | bc)
fi

for ((i=0;i<$TEST_ANNOTATIONS_COUNT;i++)); do
	RIGHT_EYE_PUPIL_ERROR=$(echo "$RIGHT_EYE_PUPIL_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
	LEFT_EYE_PUPIL_ERROR=$(echo "$LEFT_EYE_PUPIL_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
	if [[ BASIC ]]; then
		RIGHT_MOUTH_ERROR=$(echo "$RIGHT_MOUTH_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_MOUTH_ERROR=$(echo "$LEFT_MOUTH_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_OUTER_EYEBROW_ERROR=$(echo "$RIGHT_OUTER_EYEBROW_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_INNER_EYEBROW_ERROR=$(echo "$RIGHT_INNER_EYEBROW_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_INNER_EYEBROW_ERROR=$(echo "$LEFT_INNER_EYEBROW_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_OUTER_EYEBROW_ERROR=$(echo "$LEFT_OUTER_EYEBROW_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_TEMPLE_ERROR=$(echo "$RIGHT_TEMPLE_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_OUTER_EYE_ERROR=$(echo "$RIGHT_OUTER_EYE_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_INNER_EYE_ERROR=$(echo "$RIGHT_INNER_EYE_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_INNER_EYE_ERROR=$(echo "$LEFT_INNER_EYE_ERROR+(${RIGHT_EYE_PUPIL_DISTANCES[$i]}-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_OUTER_EYE_ERROR=$(echo "$LEFT_OUTER_EYE_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_TEMPLE_ERROR=$(echo "$LEFT_TEMPLE_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		RIGHT_NOSTRIL_ERROR=$(echo "$RIGHT_NOSTRIL_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		LEFT_NOSTRIL_ERROR=$(echo "$LEFT_NOSTRIL_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		CENTRE_POINT_OF_LOWER_LIP_ERROR=$(echo "$CENTRE_POINT_OF_LOWER_LIP_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
		TIP_OF_CHIN_ERROR=$(echo "$TIP_OF_CHIN_ERROR+($RIGHT_EYE_PUPIL_ERROR-$RIGHT_EYE_PUPIL_MEAN)^2" | bc)
fi
done

# DISPLAY RESULTS

