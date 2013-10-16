project(particles2d)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

subdirs(src)
