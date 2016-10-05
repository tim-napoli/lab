/**
 * @module      lab::event::event
 * @brief       Event Listener interface.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_event_listener_hpp_
#define _lab_event_listener_hpp_

#include "lab/util/exception.hpp"
#include "lab/event/event.hpp"

namespace lab { namespace event {

/**
 * The listener interface is derived when an object should be able to be
 * connected to event sources.
 */
class listener {
  public:
    virtual void notify(const event& event) throw(util::exception) = 0;
};

}}

#endif

