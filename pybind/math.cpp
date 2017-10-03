#include <pybind11/pybind11.h>
#include "glm-cast.hpp"
#include "lab/math/box.hpp"
#include "lab/math/transform.hpp"
#include "lab/math/segment.hpp"

using namespace lab;

namespace py = pybind11;

PYBIND11_MODULE(math, m) {
    m.doc() = "lab::math python binding.";

    py::class_<math::box>(m, "box")
        .def(py::init<glm::vec2, glm::vec2>())
        .def(py::init<float, float, float, float>())
        .def("left",            &math::box::left)
        .def("right",           &math::box::right)
        .def("bottom",          &math::box::bottom)
        .def("top",             &math::box::top)
        .def("bottom_left",     &math::box::bottom_left)
        .def("top_right",       &math::box::top_right)
        .def("width",           &math::box::width)
        .def("height",          &math::box::height)
        .def("get_dimensions",  &math::box::get_dimensions)
        .def("contains",        &math::box::contains)
        .def("intersects",      &math::box::intersects)
        ;

    py::class_<math::transform>(m, "transform")
        .def(py::init())
        .def(py::init<glm::vec2, glm::vec2, float>())
        .def("get_translation",     &math::transform::get_translation)
        .def("get_scale",           &math::transform::get_scale)
        .def("get_rotation",        &math::transform::get_rotation)
        .def("set_translation",     &math::transform::set_translation)
        .def("set_scale",           &math::transform::set_scale)
        .def("set_rotation",        &math::transform::set_rotation)
        .def("apply",               &math::transform::apply)
        .def("to_matrix",           &math::transform::to_matrix)
        ;
}

