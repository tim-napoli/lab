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

pybind11_add_module(labgfx MODULE gfx.cpp)
target_link_libraries(
    labgfx PRIVATE
    lab-math
    lab-gfx
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARIES}
)
pybind11_add_module(labdata MODULE data.cpp)
target_link_libraries(
    labdata PRIVATE
    lab-data
    lab-gfx
    lab-json
    lab-parser
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARIES}
)

