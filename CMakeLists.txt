cmake_minimum_required(VERSION 3.21.0)

project("learncpp" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(INC_DIR include)
include_directories(${INC_DIR})

add_executable(main main.cpp)
add_executable(gtest test.cpp)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(gtest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(NAME AllTests COMMAND gtest)

