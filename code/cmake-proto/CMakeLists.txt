cmake_minimum_required(VERSION 2.8)
project(DEDUPE)

# define & doc project properties; global/directory/target...
#define_property( GLOBAL)

# add global compiler options
add_compile_options(-O2 -g)

# and definitions
add_definitions(-DFOO)

# add src dir & build it into bin
add_subdirectory(src bin)
