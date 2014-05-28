cmake_minimum_required(VERSION 2.6)

project(decor3d)

set (DECOR3D_VERSION_MAJOR 0)
set (DECOR3D_VERSION_MINOR 1)

set (CMAKE_INSTALL_PREFIX ../bin)
INSTALL( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/decor3d DESTINATION . )
set( CMAKE_BUILD_TYPE Debug )

find_package( OpenCV REQUIRED )

file(GLOB cpp
	"*.cpp"
	"frame-annotator/*.cpp"
)

add_executable(decor3d ${cpp})

target_link_libraries( decor3d ${OpenCV_LIBS} )
