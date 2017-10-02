#include <pybind11/pybind11.h>
#include "lab/engine/engine.hpp"

using namespace lab;

namespace py = pybind11;

class py_screen : public engine::screen {
  public:
    py_screen() { }

    ~py_screen() { }

    void start(engine::engine_interface* intf)
        throw(util::exception) override
    {
        PYBIND11_OVERLOAD_PURE(
            void,
            engine::screen,
            start,
            intf
        );
    }

    void update(engine::engine_interface* intf)
        throw(util::exception) override
    {
        PYBIND11_OVERLOAD_PURE(
            void,
            engine::screen,
            update,
            intf
        );
    }

    void stop(engine::engine_interface* intf)
        throw(util::exception) override
    {
        PYBIND11_OVERLOAD_PURE(
            void,
            engine::screen,
            stop,
            intf
        );
    }

    void notify(const event::event& evt)
        throw(util::exception) override
    {
        PYBIND11_OVERLOAD_PURE(
            void,
            engine::screen,
            notify,
            evt
        );
    }
};

PYBIND11_MODULE(labengine, m) {
    m.doc() = "lab::util python binding.";

    py::class_<engine::engine>(m, "engine")
        .def(py::init<const std::string&, int, int, bool, int>())
        .def("start", &engine::engine::start)
        .def("run", &engine::engine::run)
        .def("stop", &engine::engine::stop)
        .def("start_screen", &engine::engine::start_screen, py::keep_alive<1, 2>())
        .def("close_screen", &engine::engine::close_screen)
        .def("get_window_width", &engine::engine::get_window_width)
        .def("get_window_height", &engine::engine::get_window_height)
        .def("notify", &engine::engine::notify)
        ;

    py::class_<engine::screen, py_screen> screen(m, "screen");
    screen
        .def(py::init())
        .def("start",  &engine::screen::start)
        .def("stop",   &engine::screen::stop)
        .def("update", &engine::screen::update)
        .def("notify", &engine::screen::notify)
        ;
}


