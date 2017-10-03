#include <pybind11/pybind11.h>
#include "glm-cast.hpp"
#include "lab/data/manifest.hpp"

using namespace lab;

namespace py = pybind11;

PYBIND11_MODULE(labdata, m) {
    m.doc() = "lab::data python binding.";

    py::class_<data::manifest>(m, "manifest")
        .def(py::init<>())
        .def("get_texture",     &data::manifest::get_texture)
        .def("get_image",       &data::manifest::get_image)
        .def("get_image_ptr",   &data::manifest::get_image_ptr,
             py::return_value_policy::reference)
        .def("get_animation",   &data::manifest::get_animation)
        .def("destroy",         &data::manifest::destroy)
        .def_static("load",     &data::manifest::load)
        ;
}

