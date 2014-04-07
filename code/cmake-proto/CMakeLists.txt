cmake_minimum_required(VERSION 2.8)
project(DEDUPE)

# required packages
# see http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/
find_package( Boost 1.44 COMPONENTS filesystem REQUIRED )
find_package( Git 1.8 REQUIRED )

# define & doc project properties; global/directory/target...
#define_property( GLOBAL)

# add global compiler options
add_compile_options(-O2 -g)

# and definitions
add_definitions(-DFOO)

# add src dir & build it into bin
add_subdirectory(src bin)
