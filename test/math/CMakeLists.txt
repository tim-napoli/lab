add_library(
    test-math
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
)

CXXTEST_ADD_TEST(
    segment
    segment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/segment.hpp
)

CXXTEST_ADD_TEST(
    triangle
    triangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/triangle.hpp
)

target_link_libraries(segment lab-math test-math)
target_link_libraries(triangle lab-math test-math)
