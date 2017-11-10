cmake_minimum_required(VERSION 3.9)
project(LRT)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/LindenbergResearch.cpp
        src/LindenbergResearch.hpp
        src/SimpleFilter.cpp
        src/SimpleFilter.hpp)

include_directories(.)
include_directories(src)
include_directories(../../include)

add_executable(LRT ${SOURCE_FILES})