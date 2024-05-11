#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
DEFAULT='\033[0m'

# Define the parent directory containing 'working' and 'error'
scriptDir="$(dirname $(readlink -f $0))"
parentDir="$(dirname "$scriptDir")"

parent_dir="$parentDir/test/phase2_tests"
result_dir="$parentDir/test_results/Phase2"

if [ ! -d $parent_dir ]; then
	echo "test dir $parent_dir does not exist"
	exit 1
fi

rm -rf $result_dir
# Create the 'result' subdirectory if it doesn't exist
mkdir -p "$result_dir/Working"
mkdir -p "$result_dir/Errors"
mkdir -p "$result_dir/Logs"
mkdir -p "$result_dir/Quads"

# Loop through all subdirectories within the parent directory
for subdir in "$parent_dir"/*/; do
    # Extract the base directory name (without path)
    base_dirname=$(basename "$subdir")
	
	i=0
	echo "=================== $base_dirname ======================"

    # Loop through all .asc files in the current subdirectory
    for input_file in "$subdir"/*.asc; do
        # Extract the base filename (without extension)
        base_filename=$(basename "$input_file" .asc)

		printf "%-3d %-25.25s: " "$i" "$base_filename"
		i=$((i+1))

		# Run the program with the input file
       "$parentDir"/bin/parser.out "$input_file" "$result_dir/${base_dirname}/test_${base_filename}" 2>"$result_dir/Logs/log_${base_filename}.out"

		if [ -e quads.txt ]; then
			echo -e "${GREEN}COMPILATION SUCCESS${DEFAULT}"
			mv quads.txt $result_dir/Quads/$base_filename.quad
		else
			echo -e "${RED}COMPILATION FAILED${DEFAULT}"
		fi

    done
done