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
#include "lab/engine/engine-interface.hpp"

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
class screen : public event::listener

{
  public:
    virtual ~screen() { }

    virtual void start(engine_interface* intf) throw(util::exception) = 0;

    virtual void update(engine_interface* intf) throw(util::exception) = 0;

    virtual void stop(engine_interface* intf) throw(util::exception) = 0;
};

}}

#endif

