#!/bin/bash

# Define the parent directory containing 'working' and 'error'
parent_dir="test/phase2_tests"
result_dir="test_results/Phase2"

rm -rf $result_dir
# Create the 'result' subdirectory if it doesn't exist
mkdir -p "$result_dir/Working"
mkdir -p "$result_dir/Errors"
mkdir -p "$result_dir/Logs"

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
       ./bin/parser.out "$input_file" "$result_dir/${base_dirname}/test_${base_filename}" 2>"$result_dir/Logs/log_${base_filename}.out"
    done
done