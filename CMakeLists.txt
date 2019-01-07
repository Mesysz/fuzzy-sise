cmake_minimum_required (VERSION 2.6)
project (fuzzy-sise)

set(CMAKE_CXX_STANDARD 17)

#include(SFML-2.5.1/CMakeLists.txt)

add_executable(fuzzy-sise main.cpp Road.cpp Road.h functions.cpp functions.h FuzzyDriver.cpp FuzzyDriver.h
        tinyxml2-master/tinyxml2.cpp tinyxml2-master/tinyxml2.h)

