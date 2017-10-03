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

set(PYLAB_DIR "${PROJECT_BINARY_DIR}/pylab")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/pylab")
file(WRITE "${PROJECT_BINARY_DIR}/pylab/__init__.py" "")

macro(copy_py_lib name)
    add_custom_command(
        TARGET lab${name}
        POST_BUILD
        COMMAND cp $<TARGET_FILE:lab${name}> "${PYLAB_DIR}/${name}.so"
    )
endmacro(copy_py_lib)

copy_py_lib(util)
copy_py_lib(event)
copy_py_lib(math)
copy_py_lib(engine)
copy_py_lib(data)
copy_py_lib(gfx)

