cmake_minimum_required(VERSION 3.16)
project(task1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(task1 main.cpp Image.cpp)
        
enable_testing()
add_executable(mytest test.cpp)
target_link_libraries(
  mytest
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(mytest)


