cmake_minimum_required(VERSION 2.8)
project(SDES)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES main.cpp include/SDES/SDES.h src/SDES.cpp)
add_executable(SDES ${SOURCE_FILES})
