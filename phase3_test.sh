#!/bin/bash

# Define the parent directory containing 'working' and 'error'
parent_dir="test/phase3_tests"
result_dir="test_results/Phase3"

rm -rf $result_dir
# Create the 'result' subdirectory if it doesn't exist
mkdir -p "$result_dir/Logs"
mkdir -p "$result_dir/SymTable"
mkdir -p "$result_dir/Quads"

for input_file in "$parent_dir"/*; do
# Extract the base filename (without extension)
	base_filename=$(basename "$input_file" .asc)

	printf "%-3d %-25.25s: " "$i" "$base_filename"
	i=$((i+1))
	
	# Run the program with the input file
	./bin/parser.out "$input_file" "$result_dir/SymTable/test_${base_filename}" 2>"$result_dir/Logs/log_${base_filename}.out"
	mv quads.txt $result_dir/Quads/$base_filename.quad
done