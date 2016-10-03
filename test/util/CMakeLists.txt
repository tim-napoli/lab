CXXTEST_ADD_TEST(
    exception
    exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/exception.hpp
)

CXXTEST_ADD_TEST(
    value
    value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/value.hpp
)
