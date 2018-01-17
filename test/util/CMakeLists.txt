CXXTEST_ADD_TEST(
    value
    value.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/value.hpp
)

CXXTEST_ADD_TEST(
    result
    result.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result.hpp
)

CXXTEST_ADD_TEST(
    string
    string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/string.hpp
)
