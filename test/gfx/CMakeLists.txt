CXXTEST_ADD_TEST(
    renderer
    renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.hpp
)
target_link_libraries(renderer
                      lab-engine
                      lab-json
                      lab-parser
                      lab-event
                      lab-gfx
                      mock-gl
                      mock-glfw
                      pthread)


