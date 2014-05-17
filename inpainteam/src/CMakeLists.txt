cmake_minimum_required(VERSION 2.6)

project(decor3d)

set (DECOR3D_VERSION_MAJOR 0)
set (DECOR3D_VERSION_MINOR 1)

set (CMAKE_INSTALL_PREFIX ../bin)
INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/decor3d DESTINATION . )

file(GLOB cpp
	"*.cpp"
)

add_executable(decor3d ${cpp})
