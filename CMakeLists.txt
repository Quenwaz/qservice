# CMakeList.txt : CMake project for qservice, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.18)


# CMAKE Pin cxx compiler to CXX14 until update to CXX17
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (qservice LANGUAGES C CXX)

include(GNUInstallDirs)

add_compile_options(-Wall -Wconversion -pedantic -Wextra -Wno-missing-field-initializers)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-g -Og -ftest-coverage -fstack-protector-all --param=ssp-buffer-size=4)
endif()



option(BUILD_SHARED_LIBS "build shared library" ON)
option(QSERVICE_BUILD_TESTS "build tests alongside the project" OFF)
option(QSERVICE_BUILD_EXAMPLES "build examples alongside the project" OFF)
option(QSERVICE_INSTALL "add qservice as install target (recommended)" ON)
# Add source to this project's executable.
add_subdirectory(src)



# TODO: Add tests and install targets if needed.

if (QSERVICE_BUILD_TESTS)
    include(CTest)
    find_package(GTest QUIET)
    if (NOT GTEST_FOUND)
    message("GoogleTest not found. Consider installing it on your system. Downloading it from source...")
    include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_SHALLOW true
        )
        set(BUILD_GMOCK OFF CACHE BOOL "")
        FetchContent_GetProperties(googletest)
        if(NOT googletest_POPULATED)
            FetchContent_Populate(googletest)
            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
        endif()
    endif()
    add_subdirectory(tests)
endif()

message(STATUS INCLUDE_DIRECTORIES=${INCLUDE_DIRECTORIES})
