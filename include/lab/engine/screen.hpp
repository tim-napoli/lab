/**
 * @module      lab::engine::screen
 * @brief       The engine screen class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_screen_hpp_
#define _lab_engine_screen_hpp_

#include "lab/event/event.hpp"
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"

namespace lab { namespace engine {

/**
 * A screen can be viewed as a part of the game. It is an engine class
 * overloaded by the developer to do some part of the game, like a splash
 * screen or the main menu.
 *
 * A screen is aware of input devices of the engine through its 'notify'
 * method.
 *
 * A screen can order the engine to start a new screen or to close itself
 * using the 'screen::events' events.
 *
 * TODO let a screen be aware of the engine configuration (window dimensions,
 *      ticks / seconds, etc).
 */
class screen : public event::source
             , public event::listener

{
  public:
    enum events : event::event::type {
        /**
         * Sent to order the engine to close this screen.
         */
        close,

        /**
         * Sent to order the engine to start a new screen.
         * @param activity a pointer to the screen to start.
         */
        start_screen,
    };

    virtual void start() throw(util::exception) = 0;

    virtual void update() throw(util::exception) = 0;

    virtual void stop() throw(util::exception) = 0;

    SOURCE_MAKE_ID(screen)
};

}}

#endif

