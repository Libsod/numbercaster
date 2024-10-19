cmake_minimum_required(VERSION 3.21.0)

project("numbercaster" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

add_executable(main ${SRC_FILES})

list(REMOVE_ITEM SRC_FILES ${SRC_DIR}/main.cpp)
add_executable(gtest ${TEST_FILES} ${SRC_FILES})

target_include_directories(main PRIVATE ${INC_DIR} ${SRC_DIR})
target_include_directories(gtest PRIVATE ${INC_DIR} ${SRC_DIR})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(gtest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

enable_testing()
add_test(NAME AllTests COMMAND gtest)

