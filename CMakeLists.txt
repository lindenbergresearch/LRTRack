cmake_minimum_required(VERSION 3.8)
project(LRT)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/LindenbergResearch.cpp
        src/LindenbergResearch.hpp
        src/SimpleFilter.cpp
        src/SimpleFilter.hpp
        src/ReShaper.cpp
        src/ReShaper.hpp
        src/BlankPanel.cpp
        src/BlankPanel.hpp
        src/VCO.cpp
        src/VCO.hpp
        src/dsp/helper.cpp
        src/dsp/helper.hpp src/Release.h)

include_directories(.)
include_directories(src)
include_directories(src/dsp)
include_directories(../../include)

add_executable(LRT ${SOURCE_FILES} src/dsp/helper.cpp src/dsp/helper.hpp src/Release.h)