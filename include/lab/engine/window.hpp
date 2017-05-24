/**
 * @module      lab::engine::window
 * @brief       The Window module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_window_hpp_
#define _lab_engine_window_hpp_

#include <GLFW/glfw3.h>
#include "lab/engine/module.hpp"

namespace lab { namespace engine {

/**
 * Module that handles the window.
 */
class window : public event::source
{
  private:
    GLFWwindow* _window;
    std::string _name;
    int         _width, _height;
    bool        _fullscreen;

    bool monitor_is_compatible_size(GLFWmonitor* monitor,
                                    int width, int height);

  public:
    enum events : event::event::type {
        /**
         * Sent when the window received the order to close from the
         * system.
         */
        closed,

        /**
         * Sent when the window has been resized.
         * @param width the new window's width (in pixels).
         * @param height the new window's height (in pixels).
         */
        resized,
    };

    window(const std::string& name, int width, int height, bool fullscreen);

    ~window();

    /**
     * Resize the window. Make the window sending a window::events::resized
     * event.
     */
    void resize(int width, int height);

    void set_size(int width, int height);

    /**
     * Close the window. Make the window sending a window::events::closed
     * event *at the next call by the engine of window::update*.
     */
    void close();

    void start() throw(util::exception);

    void update() throw(util::exception);

    void stop() throw(util::exception);

    int get_width() const {return _width;}

    int get_height() const {return _height;}

    SOURCE_MAKE_ID(window)
};

}}

#endif

