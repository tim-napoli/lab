add_executable(
    shadow-render
    main.cpp
    renderer.cpp
    screen.cpp
    point.cpp
)

target_link_libraries(
    shadow-render
    lab-engine
    lab-event
    lab-gfx
    lab-data
    lab-json
    lab-parser
    lab-math
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARIES}
)
