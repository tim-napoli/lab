/**
 * @module      lab::event::source
 * @brief       The event source class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_event_source_hpp_
#define _lab_event_source_hpp_

#include <list>
#include "lab/event/event.hpp"
#include "lab/event/listener.hpp"

namespace lab { namespace event {

/**
 * An event source is an object that will generate lab events. An event source
 * has some registred listeners (that are other objects) that will receive
 * every event sent by the source.
 */
class source {
  private:
    std::list<listener*> _listeners;

  public:
    source();

    virtual ~source();

    /**
     * Add a new listener to the source.
     * @pre l is not NULL.
     */
    void register_listener(listener* l);

    /**
     * Remove given listener from the source. Do nothing if the listener
     * doesn't exist.
     */
    void unregister_listener(listener* l);

    /**
     * Send the given event from this source to every of its listeners.
     * @throw Throws exceptions thrown by listeners at event reception.
     */
    void send_event(const event& event) throw(util::exception);
};

}}

#endif

