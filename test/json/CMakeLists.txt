CXXTEST_ADD_TEST(
    json
    json.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp
)
target_link_libraries(json lab-json lab-parser)
