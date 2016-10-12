CXXTEST_ADD_TEST(
    parser
    parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp
)
target_link_libraries(parser lab-parser)
