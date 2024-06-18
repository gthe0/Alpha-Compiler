#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
DEFAULT='\033[0m'

# Define the parent directory containing 'working' and 'error'
scriptDir="$(dirname $(readlink -f $0))"
parentDir="$(dirname "$scriptDir")"

parent_dir="$parentDir/test/phase45_tests"
result_dir="$parentDir/test_results/Phase45"

if [ ! -d $parent_dir ]; then
	echo "test dir $parent_dir does not exist"
	exit 1
fi

rm -rf $result_dir
# Create the 'result' subdirectory if it doesn't exist
mkdir -p "$result_dir/Logs"
mkdir -p "$result_dir/SymTable"
mkdir -p "$result_dir/Quads"
mkdir -p "$result_dir/TCG"
mkdir -p "$result_dir/Binary"

for input_file in "$parent_dir"/*; do
	# Extract the base filename (without extension)
	base_filename=$(basename "$input_file" .asc)

	printf "%-3d %-25.25s: " "$i" "$base_filename"
	i=$((i+1))
	
	# Run the program with the input file
    "$parentDir"/bin/parser.out "-siq" "-O3" "$input_file" 2> "$result_dir/Logs/${base_filename}.log"

	# Move the file with the symbol table entries into its corresponding folder
	mv sym_table.txt "$result_dir/SymTable/test_${base_filename}"

	if [ -e a.abc ]; then
		echo -e "${GREEN}COMPILATION SUCCESS${DEFAULT}"
		mv a.abc $result_dir/Binary/$base_filename.abc

		if [ -e tcg_instructions.txt ]; then
			mv tcg_instructions.txt $result_dir/TCG/$base_filename.instr
		fi

		if [ -e quads.txt ]; then
			mv quads.txt $result_dir/Quads/$base_filename.quad
		fi
		
	else
		echo -e "${RED}COMPILATION FAILED${DEFAULT}"
	fi
	
done
