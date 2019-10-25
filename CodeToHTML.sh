#!/bin/bash
# Purpose: 	This script will recursively grab all the .c and .h files
# 			from the provided path and send it to hilite.me to compile
# 			it into a single formatted html file.
# Note:		This will grab EVERY .c and .h file recursively from the
# 			provided path. It might be wise to copy only the files you
# 			need to submit somewhere else so it doesn't grab files you
# 			don't want to submit (like cmake files)

OUTPUT_FILE="submission.html"

# Check for dependencies
type wget >/dev/null 2>&1 || { echo >&2 "I require wget but it's not installed.  Aborting."; exit 1; }

# Check for parameter
if [ $# -eq 0 ]
then
    echo "Usage: ./CodeToHTML.sh <path to base code directory>"
	exit 1
fi
DIRECTORY=$1 # Path to directory to recursively check for .h's and .c's

# Start GET parameters
printf 'code=' > out1.txt #beginning of GET

# Print each file to out1.txt
for f in $(find "${DIRECTORY}" -name '*.c' -or -name '*.h')
do
	# Add file header
	printf '/****************************/\r\n/* %s */\r\n/***************************/\r\n' $f >> out1.txt
	# Add file contents
	cat $f >> out1.txt
	# Add file seperator
	printf '\r\n\r\n\r\n\r\n\r\n' >> out1.txt
done

# Replace all &'s with %26 for GET call
cat out1.txt | sed -r 's/&/\%26/g' > out2.txt
# Replace all +'s with %2B for GET call
cat out2.txt | sed -r 's/\+/\%2B/g' > out3.txt
# Finish off parameters
printf '&lexer=c&options=&style=friendly&linenos=1&divstyles=' >> out3.txt

# Send GET request to hilite.me for a formatted HTML file
wget --post-file out3.txt -O $OUTPUT_FILE http://hilite.me/api

# Cleanup
rm out1.txt
rm out2.txt
rm out3.txt
