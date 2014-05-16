cmake_minimum_required (VERSION 2.6)

project (decor3d)

set (DECOR3D_VERSION_MAJOR 0)
set (DECOR3D_VERSION_MINOR 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


# configure header file to pass some of the CMake settings
# to the source code
# configure_file (
# "${PROJECT_SOURCE_DIR}/Config.in"
#  "${PROJECT_BINARY_DIR}/Config.h"
#  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# include_directories("${PROJECT_BINARY_DIR}")

# add the executable
##add_executable(decor3d main.cpp)

add_subdirectory(src)
