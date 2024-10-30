cmake_minimum_required(VERSION 3.12)
set(CXX_STANDARD_REQUIRED 11)

# Old MacOS sometimes seems to not get CXX_FLAGS set correctly
# on use of CXX_STANDARD_REQUIRED
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "Debug")
    message("Building Debug...")
else()
    message("Building Release...")
endif()

project(DataExporter)

set(DEX_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(DEX_GET_DIR "/Users/attpc/Documents/GET-software/") # Don't @me for this.
find_package(Ice COMPONENTS Ice)
find_package(Boost REQUIRED)

add_subdirectory(src)