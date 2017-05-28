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

CXXTEST_ADD_TEST(
    box
    box.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/box.hpp
)

target_link_libraries(segment test-math lab-math)
target_link_libraries(triangle test-math lab-math)
target_link_libraries(box test-math lab-math)
