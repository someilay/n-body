cmake_minimum_required(VERSION 3.22)
project(n_body VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Set Release as default")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    message(STATUS "Disabling CMAke extensions")
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if (NOT SFML_DIR)
    message(FATAL_ERROR "Specify SFML_DIR!")
endif()

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED PATHS ${SFML_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add include directories
include_directories(SFML/include include)
link_directories(SFML/lib)

# add the source directories
file(GLOB SRCS src/*.cpp)

add_executable(n_body ${SRCS})

target_link_libraries(n_body sfml-system sfml-window sfml-graphics)
