#!/bin/bash
# This code takes all .c and .h files in a directory and converts them into a single .pdf file
#  with nice color formatting

type enscript >/dev/null 2>&1 || { echo >&2 "I require enscript but it's not installed.  Aborting."; exit 1; }
type ps2pdf >/dev/null 2>&1 || { echo >&2 "I require ghostscript but it's not installed.  Aborting."; exit 1; }

# echo "All required packages installed"


if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    echo "Usage: $0 directory"
    exit 1
fi

code_dir=$1

echo "Looking in $code_dir:"
output_name="formatted_code.pdf"

# Get all the .h and .c files in the directory, switch the .h to be in front of the .c, then
#  make it look nice and export to pdf format
find $code_dir/* -name "*.[hc]" | sed '/CMakeFiles/d' | sort -r |  xargs enscript --color=1 --tabsize=4 -C -B -M Letter -Ec -fCourier8 -o - | ps2pdf - $output_name
