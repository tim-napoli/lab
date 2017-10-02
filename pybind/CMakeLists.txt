find_package(PythonLibs)
if (NOT PYTHONLIBS_FOUND)
    message(WARNING "Python is missing: cannot build labengine python binding")
endif()

add_subdirectory(pybind11)

pybind11_add_module(labutil MODULE util.cpp)

pybind11_add_module(labevent MODULE event.cpp)
target_link_libraries(labevent PRIVATE lab-event)
