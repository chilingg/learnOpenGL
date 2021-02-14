cmake_minimum_required(VERSION 3.1)

add_executable(uniform uniform.cpp)
target_link_libraries(uniform redopera)

find_file(TESTFILE test.cpp .)
if(TESTFILE)
    add_executable(test test.cpp)
    target_link_libraries(test redopera)
endif()
