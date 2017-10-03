find_package(PythonLibs)
if (NOT PYTHONLIBS_FOUND)
    message(WARNING "Python is missing: cannot build labengine python binding")
endif()

add_subdirectory(pybind11)

pybind11_add_module(labutil MODULE util.cpp)

pybind11_add_module(labevent MODULE event.cpp)
target_link_libraries(labevent PRIVATE lab-event)

pybind11_add_module(labengine MODULE engine.cpp)
target_link_libraries(
    labengine PRIVATE
    lab-engine
    lab-event
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
)

pybind11_add_module(labmath MODULE math.cpp)
target_link_libraries(
    labmath PRIVATE
    lab-math
)

