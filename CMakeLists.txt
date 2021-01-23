cmake_minimum_required(VERSION 3.1)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_file(REDOPERA redopera/CMakeLists.txt .)
if(REDOPERA)
	add_subdirectory(redopera redopera)
	include_directories(redopera/include/redopera)
	link_directories(${CMAKE_BINARY_DIR}/redopera/src)
endif()

add_subdirectory(learn learn)


