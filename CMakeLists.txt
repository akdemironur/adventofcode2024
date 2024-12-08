cmake_minimum_required(VERSION 3.15)
project(advent_of_code)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_library(aoc_utils INTERFACE)
target_include_directories(aoc_utils INTERFACE ${CMAKE_SOURCE_DIR}/include)

file(GLOB AOC_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

foreach(source_file ${AOC_SOURCES})
    get_filename_component(DAY ${source_file} NAME_WE)
    add_executable(${DAY} ${source_file})
    target_link_libraries(${DAY} PRIVATE aoc_utils)
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/inputs/${DAY}.txt")
        add_custom_command(
            TARGET ${DAY} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/inputs/${DAY}.txt
                ${CMAKE_BINARY_DIR}/inputs/${DAY}.txt
        )
    endif()
endforeach()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/inputs)

