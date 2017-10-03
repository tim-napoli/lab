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

    py::class_<engine::window> engine_window(m, "window");
    engine_window
        .def_static("get_id", &engine::window::get_id)
        ;
    py::enum_<engine::window::events>(engine_window, "events")
        .value("closed",    engine::window::events::closed)
        .value("resized",   engine::window::events::resized)
        ;

    py::class_<engine::keyboard> engine_keyboard(m, "keyboard");
    engine_keyboard
        .def_static("get_id", &engine::keyboard::get_id)
        ;
    py::enum_<engine::keyboard::events>(engine_keyboard, "events")
        .value("key_pressed", engine::keyboard::events::key_pressed)
        .value("key_repeated", engine::keyboard::events::key_repeated)
        .value("key_released", engine::keyboard::events::key_released)
        ;

    py::enum_<engine::keyboard::key_codes>(engine_keyboard, "key_codes")
// key codes ------------------------------------------------------------------
        .value("key_unknown", engine::keyboard::key_codes::key_unknown)
        .value("key_space", engine::keyboard::key_codes::key_space)
        .value("key_apostrophe", engine::keyboard::key_codes::key_apostrophe)
        .value("key_comma", engine::keyboard::key_codes::key_comma)
        .value("key_minus", engine::keyboard::key_codes::key_minus)
        .value("key_period", engine::keyboard::key_codes::key_period)
        .value("key_slash", engine::keyboard::key_codes::key_slash)
        .value("key_0", engine::keyboard::key_codes::key_0)
        .value("key_1", engine::keyboard::key_codes::key_1)
        .value("key_2", engine::keyboard::key_codes::key_2)
        .value("key_3", engine::keyboard::key_codes::key_3)
        .value("key_4", engine::keyboard::key_codes::key_4)
        .value("key_5", engine::keyboard::key_codes::key_5)
        .value("key_6", engine::keyboard::key_codes::key_6)
        .value("key_7", engine::keyboard::key_codes::key_7)
        .value("key_8", engine::keyboard::key_codes::key_8)
        .value("key_9", engine::keyboard::key_codes::key_9)
        .value("key_semicolon", engine::keyboard::key_codes::key_semicolon)
        .value("key_equal", engine::keyboard::key_codes::key_equal)
        .value("key_a", engine::keyboard::key_codes::key_a)
        .value("key_b", engine::keyboard::key_codes::key_b)
        .value("key_c", engine::keyboard::key_codes::key_c)
        .value("key_d", engine::keyboard::key_codes::key_d)
        .value("key_e", engine::keyboard::key_codes::key_e)
        .value("key_f", engine::keyboard::key_codes::key_f)
        .value("key_g", engine::keyboard::key_codes::key_g)
        .value("key_h", engine::keyboard::key_codes::key_h)
        .value("key_i", engine::keyboard::key_codes::key_i)
        .value("key_j", engine::keyboard::key_codes::key_j)
        .value("key_k", engine::keyboard::key_codes::key_k)
        .value("key_l", engine::keyboard::key_codes::key_l)
        .value("key_m", engine::keyboard::key_codes::key_m)
        .value("key_n", engine::keyboard::key_codes::key_n)
        .value("key_o", engine::keyboard::key_codes::key_o)
        .value("key_p", engine::keyboard::key_codes::key_p)
        .value("key_q", engine::keyboard::key_codes::key_q)
        .value("key_r", engine::keyboard::key_codes::key_r)
        .value("key_s", engine::keyboard::key_codes::key_s)
        .value("key_t", engine::keyboard::key_codes::key_t)
        .value("key_u", engine::keyboard::key_codes::key_u)
        .value("key_v", engine::keyboard::key_codes::key_v)
        .value("key_w", engine::keyboard::key_codes::key_w)
        .value("key_x", engine::keyboard::key_codes::key_x)
        .value("key_y", engine::keyboard::key_codes::key_y)
        .value("key_z", engine::keyboard::key_codes::key_z)
        .value("key_left_bracket", engine::keyboard::key_codes::key_left_bracket)
        .value("key_backslash", engine::keyboard::key_codes::key_backslash)
        .value("key_right_bracket", engine::keyboard::key_codes::key_right_bracket)
        .value("key_grave_accent", engine::keyboard::key_codes::key_grave_accent)
        .value("key_world_1", engine::keyboard::key_codes::key_world_1)
        .value("key_world_2", engine::keyboard::key_codes::key_world_2)
        .value("key_escape", engine::keyboard::key_codes::key_escape)
        .value("key_enter", engine::keyboard::key_codes::key_enter)
        .value("key_tab", engine::keyboard::key_codes::key_tab)
        .value("key_backspace", engine::keyboard::key_codes::key_backspace)
        .value("key_insert", engine::keyboard::key_codes::key_insert)
        .value("key_delete", engine::keyboard::key_codes::key_delete)
        .value("key_right", engine::keyboard::key_codes::key_right)
        .value("key_left", engine::keyboard::key_codes::key_left)
        .value("key_down", engine::keyboard::key_codes::key_down)
        .value("key_up", engine::keyboard::key_codes::key_up)
        .value("key_page_up", engine::keyboard::key_codes::key_page_up)
        .value("key_page_down", engine::keyboard::key_codes::key_page_down)
        .value("key_home", engine::keyboard::key_codes::key_home)
        .value("key_end", engine::keyboard::key_codes::key_end)
        .value("key_caps_lock", engine::keyboard::key_codes::key_caps_lock)
        .value("key_scroll_lock", engine::keyboard::key_codes::key_scroll_lock)
        .value("key_num_lock", engine::keyboard::key_codes::key_num_lock)
        .value("key_print_screen", engine::keyboard::key_codes::key_print_screen)
        .value("key_pause", engine::keyboard::key_codes::key_pause)
        .value("key_f1", engine::keyboard::key_codes::key_f1)
        .value("key_f2", engine::keyboard::key_codes::key_f2)
        .value("key_f3", engine::keyboard::key_codes::key_f3)
        .value("key_f4", engine::keyboard::key_codes::key_f4)
        .value("key_f5", engine::keyboard::key_codes::key_f5)
        .value("key_f6", engine::keyboard::key_codes::key_f6)
        .value("key_f7", engine::keyboard::key_codes::key_f7)
        .value("key_f8", engine::keyboard::key_codes::key_f8)
        .value("key_f9", engine::keyboard::key_codes::key_f9)
        .value("key_f10", engine::keyboard::key_codes::key_f10)
        .value("key_f11", engine::keyboard::key_codes::key_f11)
        .value("key_f12", engine::keyboard::key_codes::key_f12)
        .value("key_f13", engine::keyboard::key_codes::key_f13)
        .value("key_f14", engine::keyboard::key_codes::key_f14)
        .value("key_f15", engine::keyboard::key_codes::key_f15)
        .value("key_f16", engine::keyboard::key_codes::key_f16)
        .value("key_f17", engine::keyboard::key_codes::key_f17)
        .value("key_f18", engine::keyboard::key_codes::key_f18)
        .value("key_f19", engine::keyboard::key_codes::key_f19)
        .value("key_f20", engine::keyboard::key_codes::key_f20)
        .value("key_f21", engine::keyboard::key_codes::key_f21)
        .value("key_f22", engine::keyboard::key_codes::key_f22)
        .value("key_f23", engine::keyboard::key_codes::key_f23)
        .value("key_f24", engine::keyboard::key_codes::key_f24)
        .value("key_f25", engine::keyboard::key_codes::key_f25)
        .value("key_kp_0", engine::keyboard::key_codes::key_kp_0)
        .value("key_kp_1", engine::keyboard::key_codes::key_kp_1)
        .value("key_kp_2", engine::keyboard::key_codes::key_kp_2)
        .value("key_kp_3", engine::keyboard::key_codes::key_kp_3)
        .value("key_kp_4", engine::keyboard::key_codes::key_kp_4)
        .value("key_kp_5", engine::keyboard::key_codes::key_kp_5)
        .value("key_kp_6", engine::keyboard::key_codes::key_kp_6)
        .value("key_kp_7", engine::keyboard::key_codes::key_kp_7)
        .value("key_kp_8", engine::keyboard::key_codes::key_kp_8)
        .value("key_kp_9", engine::keyboard::key_codes::key_kp_9)
        .value("key_kp_decimal", engine::keyboard::key_codes::key_kp_decimal)
        .value("key_kp_divide", engine::keyboard::key_codes::key_kp_divide)
        .value("key_kp_multiply", engine::keyboard::key_codes::key_kp_multiply)
        .value("key_kp_subtract", engine::keyboard::key_codes::key_kp_subtract)
        .value("key_kp_add", engine::keyboard::key_codes::key_kp_add)
        .value("key_kp_enter", engine::keyboard::key_codes::key_kp_enter)
        .value("key_kp_equal", engine::keyboard::key_codes::key_kp_equal)
        .value("key_left_shift", engine::keyboard::key_codes::key_left_shift)
        .value("key_left_control", engine::keyboard::key_codes::key_left_control)
        .value("key_left_alt", engine::keyboard::key_codes::key_left_alt)
        .value("key_left_super", engine::keyboard::key_codes::key_left_super)
        .value("key_right_shift", engine::keyboard::key_codes::key_right_shift)
        .value("key_right_control", engine::keyboard::key_codes::key_right_control)
        .value("key_right_alt", engine::keyboard::key_codes::key_right_alt)
        .value("key_right_super", engine::keyboard::key_codes::key_right_super)
        .value("key_menu", engine::keyboard::key_codes::key_menu)
        .value("key_last", engine::keyboard::key_codes::key_last)
// ----------------------------------------------------------------------------
        ;
}


