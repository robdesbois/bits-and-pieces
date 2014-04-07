#!/bin/sh

cm_current_src_dir=$1
src_file_glob=$2

bold=$( echo -e "\033[1m")
plain=$(echo -e "\033[0m")
red=$(  echo -e "\033[31m")
green=$(echo -e "\033[32m")

# Diff the current & master file lists
diff_txt=$(
	find $cm_current_src_dir -name $src_file_glob \
	| sort \
	| diff -u0 src-list -
)

diff=$?

if [ $diff -ne 0 ]; then
	echo "${bold}Source file list has changed:"

	# Suppress diff output other than added/removed lines, and color them
	echo "$diff_txt" \
	| grep '^\(-[^-]\|+[^+]\)' \
	| sed -e "s/^-/$red-/g" \
	      -e "s/^+/$green+/g"
	
	echo $plain
fi

exit $diff
