CXXTEST_ADD_TEST(
    segment
    segment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/segment.hpp
)

target_link_libraries(segment lab-math)
