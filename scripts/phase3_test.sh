#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
DEFAULT='\033[0m'

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
    "$parentDir"/bin/parser.out "-s" "$input_file" 2> "$result_dir/Logs/${base_filename}.log"

	# Move the file with the symbol table entries into its corresponding folder
	mv sym_table.txt "$result_dir/SymTable/test_${base_filename}"

	if [ -e quads.txt ]; then
		echo -e "${GREEN}COMPILATION SUCCESS${DEFAULT}"
		mv quads.txt $result_dir/Quads/$base_filename.quad
	else
		echo -e "${RED}COMPILATION FAILED${DEFAULT}"
	fi
	
done
