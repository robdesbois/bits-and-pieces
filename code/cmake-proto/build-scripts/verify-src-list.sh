#!/bin/sh
#
# Regenerates a file list recursively given directory and globbing expression.
# If the file list matches the content of file 'src-list' in the current directory
# then exit with success; otherwise 

src_dir="$1"
src_glob="$2"

find "$src_dir" -name "$src_glob" \
| sort \
| diff -q src-list - >& /dev/null \
|| {
	echo 'Source list has changed; what do you want to do?'
	select acr in "Abort" "Continue" "Re-run CMake" ; do
		case $acr in 
	done
	false
}
