#include <pybind11/pybind11.h>
#include "lab/util/exception.hpp"
#include "lab/util/value.hpp"

using namespace lab;

namespace py = pybind11;

PYBIND11_MODULE(util, m) {
    m.doc() = "lab::util python binding.";

    py::class_<util::exception>(m, "exception")
        .def(py::init())
        .def(py::init<const std::string&>())
        .def("get_message", &util::exception::get_message)
        ;

    py::class_<util::value>(m, "value")
        .def(py::init<const std::string&>())
        .def("get_str", &util::value::get<std::string>)
        .def("set_str", &util::value::set<std::string>)
        .def("get_int", &util::value::get<int>)
        .def("set_int", &util::value::set<int>)
        .def("get_float", &util::value::get<float>)
        .def("set_float", &util::value::set<float>)
        ;
}

