cmake_minimum_required(VERSION 3.16)

project(grade_calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

file(GLOB SRC_FILES "src/*.cpp")

add_executable(grade_calc ${SRC_FILES})
target_include_directories(grade_calc PRIVATE include)

if(MSVC)
    target_compile_options(grade_calc PRIVATE /W4)
else()
    target_compile_options(grade_calc PRIVATE -O2 -Wall -Wextra)
endif()