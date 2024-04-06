cmake_minimum_required(VERSION 3.28)
project(task1)
set(CMAKE_CXX_STANDARD 17)

#[[
include(FetchContent)
FetchContent_Declare(
    googletest 
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(
  task1
  test.cpp
)
target_link_libraries(
    task1
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(test)
]]

add_executable(task1 main.cpp Image.cpp Image.hpp)

#[[
# Версия cmake
#cmake_minimum_required (VERSION 3.2)
# Определение проекта
project(MyProject)
set(CMAKE_CXX_STANDARD 17)
# Тип сборки (Debug или Release)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  MyProject
  tests.cpp
  
)
target_link_libraries(
    MyProject
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
]]
