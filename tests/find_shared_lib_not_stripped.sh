#!/bin/bash

# Define the search directory
SEARCH_DIR="/usr/lib"

# Use find to locate all files ending in .so
# We pipe to a while loop to handle paths safely
find "$SEARCH_DIR" -type f -name "*.so*" 2>/dev/null | while read -r lib; do
    # Check the output of the 'file' command
    if file "$lib" | grep -q "not stripped"; then
        echo "Found unstripped library: $lib"
        # Since we are in a pipe, we exit the subshell and the script
        kill -PIPE $$
        exit 0
    fi
done
