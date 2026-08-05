#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file=$1
nm_options=${2}
output_dir="script"
leak_dir="leak"
result_nm="resut_nm"
result_ft_nm="resut_ft_nm"
diff_dir="diff"
sanitized_file=$(echo "$file" | tr -cd '[:alnum:]_-')
ft_nm_output="${output_dir}/$result_ft_nm/result_ft_nm_${sanitized_file}"
nm_output="${output_dir}/$result_nm/result_nm_${sanitized_file}"
leak_output="${output_dir}/$leak_dir/leak_check_${sanitized_file}"
diff_output="${output_dir}/$diff_dir/diff_${sanitized_file}"

mkdir -p "$output_dir"
mkdir -p "$output_dir/$leak_dir"
mkdir -p "$output_dir/$result_nm"
mkdir -p "$output_dir/$result_ft_nm"
mkdir -p "$output_dir/$diff_dir"

# Run ft_nm and save output
./ft_nm.out $nm_options "$file" > "$ft_nm_output" 2>> "$ft_nm_output"

# Run nm and save output
nm $nm_options "$file" > "$nm_output" 2>> "$nm_output"

echo "Outputs saved to $ft_nm_output and $nm_output"

# Run leak check using valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$leak_output" ./ft_nm $nm_options "$file" > /dev/null 2>&1

echo "Leak check results saved to $leak_output"

# Check for leaks in the valgrind output
if grep -q "definitely lost:" "$leak_output"; then
    echo "Memory leaks detected in ./ft_nm. Check $leak_output for details."
else
    echo "No memory leaks detected in ./ft_nm."
fi

# Compare ft_nm and nm outputs
diff_result=$(diff "$ft_nm_output" "$nm_output")

echo "$diff_result" > "$diff_output"

if [ -z "$diff_result" ]; then
    echo "No differences found between ft_nm and nm outputs."
else
    echo "$diff_result"
    echo "Differences found: Check $diff_output"
fi

