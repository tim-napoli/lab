/**
 * @module      lab::engine::engine
 * @brief       The Lab Engine class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_engine_hpp_
#define _lab_engine_engine_hpp_

#include <memory>
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"
#include "lab/engine/module.hpp"
#include "lab/engine/window.hpp"
#include "lab/engine/keyboard.hpp"
#include "lab/engine/screen.hpp"

namespace lab { namespace engine {

/**
 * The Lab Engine (or the Great Ordonancer Of The Entire Universe And Beyond)
 * is the main class the lab library provide. It allows to do the link between
 * the system (window, keyboard, mouse...) and the game (using screens).
 * It will run at a fixed refresh rate (we say "The engine ticks every x
 * milliseconds") calling the update method of every module and of the running
 * activity.
 * The engine communicates with screen and devices using the lab event
 * system.
 */
class engine : public event::listener
{
  private:
    bool            _exit;
    double          _refresh_rate;    /// In seconds.

    window          _window;
    keyboard        _keyboard;

    std::list<screen*> _screens_stack;
    screen*            _next_screen;
    bool               _close_screen;

    void close_top_screen() throw(util::exception);

    screen* top_screen() {return _screens_stack.back();}

  public:
    /**
     * @param ticks_pers_second Times the engine ticks every seconds.
     */
    engine(const std::string& game_name,
           int window_width, int window_height, bool fullscreen,
           int ticks_per_second);

    ~engine();

    /**
     * Start the engine, every of its modules and the top activity.
     */
    void start() throw(util::exception);

    /**
     * Run the engine loops until a module or the activity order to
     * stop.
     */
    void run() throw(util::exception);

    /**
     * Stop the engine.
     */
    void stop() throw(util::exception);

    /**
     * Force the engine to stop.
     */
    void close();

    /**
     * Push a screen on the screen stack.
     */
    void push_screen(screen* scr);

    void notify(const event::event& evt) throw(util::exception);
};

}}

#endif

