/**
 * @module      lab::engine::screen_manager
 * @brief       The engine interface class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_engine_screen_manager_hpp_
#define _lab_engine_screen_manager_hpp_

#include <memory>
#include "lab/event/listener.hpp"
#include "lab/event/source.hpp"
#include "lab/engine/screen.hpp"
#include "lab/engine/engine-interface.hpp"

namespace lab { namespace engine {

enum screen_state {
    started,
    paused,
    stopped,
};

struct screen_entry {
    screen_state state;
    screen*      scr;
};

/**
 * The screen_manager is the engine part that manage screens.
 */
class screen_manager : public event::source
                     , public event::listener
{
  private:
    std::list<screen_entry> _screens;

    screen_entry& top_screen() {return _screens.back();}

  public:
    screen_manager();

    ~screen_manager();

    void update(engine_interface* intf) throw(util::exception);

    void stop(engine_interface* intf) throw(util::exception);

    void push_screen(screen* scr);

    void pop_screen(engine_interface* intf) throw(util::exception);

    bool is_screen() const {return _screens.size() > 0;}

    void notify(const event::event& evt) throw(util::exception);

    SOURCE_MAKE_ID(screen_manager)
};

}}

#endif

