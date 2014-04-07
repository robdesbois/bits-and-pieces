include_directories( ${DEDUPE_SOURCE_DIR}/include )

set(src_file_glob *.cpp)

# find the source files for this directory. docs don't recommend this as CMake can't detect
# src file add/change therefore it requires manual regeneration of makefile following a src
# list change.
# we ignore that advice here and perform our own detection of src list change
# note that the produced paths are absolute
file(
	GLOB_RECURSE dedupe_sources
	"${src_file_glob}"
)

# create the master source list file; sorted to ensure repeateable ordering in makefile
# (since CMake recursive globbing could well differ from find result)
execute_process(
	COMMAND echo "${dedupe_sources}"
	COMMAND tr '\;' '\n'
	COMMAND sort
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	OUTPUT_FILE src-list
)


# generate build target to check whether source list has changed since
# build files were last generated
add_custom_target(
	up-to-date-check
	COMMAND ${CMAKE_SOURCE_DIR}/build-scripts/up-to-date-check.sh
		${CMAKE_CURRENT_SOURCE_DIR} ${src_file_glob}
	VERBATIM
)

# add the executable with dependency on src files and src file list verifier
add_executable  (run-me ${dedupe_sources})
add_dependencies(run-me up-to-date-check)

# add a library:
# add_library(backend SHARED)
# add_library(backend STATIC)

# to add this dir *and its corresponding build dir* to 
# include path (for including generated headers), would do the following:
# set (CMAKE_INCLUDE_CURRENT_DIR ON)


# to run commands; can specify I/O files
# execute_process(COMMAND)
