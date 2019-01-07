cmake_minimum_required (VERSION 2.6)
project (fuzzy-sise)

set(CMAKE_CXX_STANDARD 17)
if (UNIX)
    set(SFML_DIR "/usr/lib/cmake/SFML")
endif (UNIX)
if (WIN32)
    set(SFML_DIR "tutaj")
endif (WIN32)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(fuzzy-sise main.cpp Road.cpp Road.h functions.cpp functions.h FuzzyDriver.cpp FuzzyDriver.h
        tinyxml2-master/tinyxml2.cpp tinyxml2-master/tinyxml2.h)
target_link_libraries(fuzzy-sise sfml-graphics sfml-audio)

