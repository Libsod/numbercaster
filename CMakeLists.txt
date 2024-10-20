# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.21.0)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "No build type specified. Setting to Debug by default.")
endif()

# Include FetchContent for downloading and managing external projects
include(FetchContent)

# Check if Ninja is available
find_program(NINJA_EXECUTABLE ninja)

# Set the generator to Ninja if available
if(NINJA_EXECUTABLE)
  set(CMAKE_GENERATOR "Ninja")
  message(STATUS "Ninja was found. Using Ninja as the build system.")
else()
  message(STATUS "Ninja is the preferred build system, but it was not found. Using the default build system")
endif()

# Check if vcpkg is already installed by finding the executable
find_program(VPCKG_EXECUTABLE vcpkg)

if(NOT VPCKG_EXECUTABLE)
    # Define the vcpkg directory relative to your project's root directory
    set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/vcpkg")

    # If vcpkg is not found, check if the directory exists
    if(NOT EXISTS "${VCPKG_DIR}")
        message(STATUS "Directory ${VCPKG_DIR} does not exist. Creating it...")
        # Create the vcpkg directory if it does not exist
        file(MAKE_DIRECTORY "${VCPKG_DIR}")

        # Fetch vcpkg from GitHub
        message(STATUS "Cloning vcpkg from GitHub...")
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git "${VCPKG_DIR}"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE git_result
        )
        
        if(git_result)
            message(FATAL_ERROR "Failed to clone vcpkg from GitHub.")
        endif()
    else()
        message(STATUS "Using existing vcpkg directory at: ${VCPKG_DIR}")
    endif()

    # Run the bootstrap script if vcpkg is available
    if(EXISTS "${VCPKG_DIR}/bootstrap-vcpkg.sh" OR EXISTS "${VCPKG_DIR}/bootstrap-vcpkg.bat")
        message(STATUS "Bootstrapping vcpkg...")
        if(UNIX)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E chdir "${VCPKG_DIR}" ./bootstrap-vcpkg.sh -disableMetrics
            )
        elseif(WIN32)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E chdir "${VCPKG_DIR}" .\\bootstrap-vcpkg.bat -disableMetrics
            )
        endif()
    else()
        message(FATAL_ERROR "Bootstrap script not found in ${VCPKG_DIR}. Try to git clone it in ${VCPKG_DIR} manually")
    endif()

    # Set the VCPKG_ROOT environment variable
    set(ENV{VCPKG_ROOT} "${VCPKG_DIR}")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    # Check if the VCPKG_ROOT environment variable is set
    if(DEFINED ENV{VCPKG_ROOT})
        message(STATUS "Using existing vcpkg found at: $ENV{VCPKG_ROOT}")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    else()
        set(VCPKG_MICROSOFT_LINK "https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell#2---set-up-the-project")
        message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to the vcpkg installation path or specify the path to the toolchain file manually (--toolchain flag). For instructions, visit: ${VCPKG_MICROSOFT_LINK}")
    endif()
endif()

# Define the project name and the language(s) it will use
project("numbercaster" LANGUAGES CXX)

# Set the C++ standard to C++23 and enforce its usage
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set optimization flags for Release and RelWithDebInfo modes
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  message(STATUS "Configuring ${CMAKE_BUILD_TYPE} mode with optimization flags.")

  # Set compiler flags for Release and RelWithDebInfo modes
  if(NOT MSVC)
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -flto")
  else()
      set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2")
  endif()
endif()

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

# Find and link the fmt package
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)

# Find and link the GTest package
find_package(GTest CONFIG REQUIRED)
target_link_libraries(gtest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

# Enable the testing framework for CMake and define a test that runs all tests
enable_testing()
add_test(NAME AllTests COMMAND gtest)
