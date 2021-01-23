cmake_minimum_required(VERSION 3.1)

find_file(TEST test.cpp .)
if(TEST)
    add_executable(test test.cpp)
    target_link_libraries(test redopera)
endif()

