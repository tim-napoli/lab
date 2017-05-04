add_executable(hello-world hello-world.cpp)
target_link_libraries(
    hello-world
    lab-engine
    lab-json
    lab-parser
    lab-event
    ${GLFW_LIBRARIES}
)
