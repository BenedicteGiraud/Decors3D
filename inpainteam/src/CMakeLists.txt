cmake_minimum_required(VERSION 2.6)

project(decor3d)

file(GLOB cpp
	"*.cpp"
)

add_executable(decor3d ${cpp})
