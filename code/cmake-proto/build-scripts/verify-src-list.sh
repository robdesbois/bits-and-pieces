#!/bin/sh
#
# Regenerates a file list recursively given directory and globbing expression.
# If the file list matches the content of file 'src-list' in the current directory
# then exit with success; otherwise 

src_dir="$1"
src_glob="$2"
cmake_binary_dir="$3"  # this is the build tree root dir
cmake_source_dir="$4"  # this is the source tree root dir

find "$src_dir" -name "$src_glob" \
| sort \
| diff -q src-list - >& /dev/null \
|| {
	echo 'Source list has changed; do you want to re-run CMake?'
	select acr in Re-run Continue Abort ; do
		case "$acr" in 
			Abort)
				echo "aborting" 
				exit 1 # failed
				break;;

			Continue)
				echo "continuing"
				exit 0 # success
				break;;

			Re-run)
				echo "re-running CMake from $cmake_binary_dir for $cmake_source_dir"
				cd "$cmake_binary_dir"
				cmake "$cmake_source_dir"
				echo "done running CMake; exiting, please re-try the build"
				exit 1 # fail to halt make; must be restarted
				break;;
		esac
	done
}
