cmake_minimum_required(VERSION 3.0)
project(labengine)

set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -fPIC ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLM REQUIRED glm)

include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(pybind)

file(COPY samples/data DESTINATION .)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    add_subdirectory(test)
endif()

