#include <pybind11/pybind11.h>
#include "lab/event/event.hpp"

using namespace lab;

namespace py = pybind11;

PYBIND11_MODULE(labevent, m) {
    m.doc() = "lab::event python binding.";

    py::class_<event::event>(m, "event")
        .def(py::init<size_t>())
        .def("get_type", &event::event::get_type)
        .def("get_source_id", &event::event::get_source_id)
        .def("set_type", &event::event::set_type)
        .def("set_value", &event::event::set_value)
        .def("get_value", &event::event::get_value)
        ;
}


