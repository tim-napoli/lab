add_executable(hello-world hello-world.cpp)
target_link_libraries(
    hello-world
    lab-engine
    lab-json
    lab-parser
    lab-event
    lab-gfx
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARIES}
)

add_executable(svg-format svg-format.cpp)
target_link_libraries(
    svg-format
    lab-math
)

add_subdirectory(shadow-render)
