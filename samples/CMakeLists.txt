add_executable(hello-world hello-world.cpp)
target_link_libraries(
    hello-world
    lab-engine
    lab-json
    lab-parser
    lab-event
    lab-gfx
    ${GLFW_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
)
