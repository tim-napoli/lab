/**
 * @module      lab::engine::window_user_data
 * @brief       GLFWwindow user data.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_window_user_data_hpp_
#define _lab_engine_window_user_data_hpp_

#include "lab/engine/window.hpp"

namespace lab { namespace engine {

struct window_user_data {
    window* win;
};

}}

#endif

