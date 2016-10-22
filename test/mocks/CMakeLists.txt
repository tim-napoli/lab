add_library(
    mock-glfw
    glfw.cpp
)

CXXTEST_ADD_TEST(
    mock-glfw-test
    glfw-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw-test.hpp
)
target_link_libraries(mock-glfw-test mock-glfw)

