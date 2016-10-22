/**
 * @module      lab::event::handler
 * @brief       Event Handler.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_event_handler_hpp_
#define _lab_event_handler_hpp_

#include <functional>
#include <map>
#include <vector>
#include "lab/util/exception.hpp"
#include "lab/event/event.hpp"
#include "lab/event/source.hpp"

namespace lab { namespace event {

/**
 * A lab::event::handler is a class that is used to define callback to
 * listeners receiving events from different sources using a fancier way than
 * a big 'switch/case' statement in the 'lab::listener::notify' method.
 * User can add more than one handler for the same event.
 */
class handler {
  public:
    typedef std::function<void(const event&)>           event_handler;
    typedef std::vector<event_handler>                  event_handlers;
    typedef std::map<event::type, event_handlers>       source_handler;

  private:
    std::map<source::id, source_handler> _handlers;

  public:
    handler();

    ~handler();

    /**
     * Add an handler for the given event source with the given event id.
     */
    void add_handler(source::id source_id, event::type event_type,
                     event_handler handler);

    /**
     * Handles the given event. Does nothing if there exist no handler for this
     * event.
     */
    void handle(const event& event) throw(util::exception);
};

}}

#endif

