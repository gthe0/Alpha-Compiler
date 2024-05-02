#!/bin/bash

# Define the parent directory containing 'working' and 'error'
scriptDir="$(dirname $(readlink -f $0))"
parentDir="$(dirname "$scriptDir")"

parent_dir="$parentDir/test/phase3_tests"
result_dir="$parentDir/test_results/Phase3"

if [ ! -d $parent_dir ]; then
	echo "test dir $parent_dir does not exist"
	exit 1
fi

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
	$parentDir/bin/parser.out "$input_file" "$result_dir/SymTable/test_${base_filename}" 2>"$result_dir/Logs/log_${base_filename}.out"
	mv quads.txt $result_dir/Quads/$base_filename.quad
done
