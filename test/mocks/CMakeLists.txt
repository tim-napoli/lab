add_library(
    mock-glfw
    glfw.cpp
)

add_library(
    mock-gl
    gl.cpp
)

CXXTEST_ADD_TEST(
    mock-glfw-test
    glfw-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw-test.hpp
)
target_link_libraries(mock-glfw-test mock-glfw pthread)

CXXTEST_ADD_TEST(
    mock-gl-test
    gl-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl-test.hpp
)
target_link_libraries(mock-gl-test mock-gl)

