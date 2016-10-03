cmake_minimum_required(VERSION 3.0)
project(labengine)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories("${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src)
add_subdirectory(test)
