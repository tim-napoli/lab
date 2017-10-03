#include <pybind11/pybind11.h>
#include "lab/event/event.hpp"
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"

using namespace lab;

namespace py = pybind11;

class py_listener : public event::listener {
  public:
    py_listener() { }

    void notify(const event::event& ev)
        throw(util::exception) override
    {
        PYBIND11_OVERLOAD_PURE(
            void,
            event::listener,
            notify,
            ev
        );
    }
};

PYBIND11_MODULE(event, m) {
    m.doc() = "lab::event python binding.";

    py::class_<event::event>(m, "event")
        .def(py::init<size_t>())
        .def("get_type", &event::event::get_type)
        .def("get_source_id", &event::event::get_source_id)
        .def("set_type", &event::event::set_type)
        .def("set_value", &event::event::set_value)
        .def("get_value", &event::event::get_value)
        ;

    py::class_<event::listener, py_listener> listener(m, "listener");
    listener
        .def(py::init())
        .def("notify", &event::listener::notify)
        ;

    py::class_<event::source>(m, "source")
        .def(py::init())
        .def("register_listener", &event::source::register_listener)
        .def("unregister_listener", &event::source::unregister_listener)
        .def("send_event", &event::source::send_event)
        .def("relay_event", &event::source::relay_event)
        ;
}


