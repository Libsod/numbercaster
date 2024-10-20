# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.21.0)

# Define the project name and the language(s) it will use
project("numbercaster" LANGUAGES CXX)

# Set the C++ standard to C++23 and enforce its usage
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the include directory for header files
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

# Set the source directory and find all .cpp files within it
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

# Set the directory for the test files and find all .cpp files within it
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

# Define the main executable and link it with the source files
add_executable(main ${SRC_FILES})

# Remove main.cpp from the list of source files when building the gtest target
list(REMOVE_ITEM SRC_FILES ${SRC_DIR}/main.cpp)
# Define the gtest executable that will run the unit tests
add_executable(gtest ${TEST_FILES} ${SRC_FILES})

# Include directories for the main executable and gtest executable
target_include_directories(main PRIVATE ${INC_DIR} ${SRC_DIR})
target_include_directories(gtest PRIVATE ${INC_DIR} ${SRC_DIR})

# Find the GTest package (installed via vcpkg or elsewhere) and link it to the gtest target
find_package(GTest CONFIG REQUIRED)
target_link_libraries(gtest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

# Enable the testing framework for CMake and define a test that runs all tests
enable_testing()
add_test(NAME AllTests COMMAND gtest)
