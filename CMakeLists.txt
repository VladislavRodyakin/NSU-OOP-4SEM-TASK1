cmake_minimum_required(VERSION 3.28)
project(task1)

SET(SOURCE_LIB Image.cpp)
SET(SOURCE_EXE main.cpp)

add_library(Image ${SOURCE_LIB})
add_executable(main ${SOURCE_EXE})

target_link_libraries(main Image)