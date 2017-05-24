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
#include <typeinfo>
#include "lab/event/event.hpp"
#include "lab/event/listener.hpp"

namespace lab { namespace event {

/**
 * This macro permits to define in every public part of lab::event::source
 * childrens a method to get a unique event source identifier.
 * This allows every event broadcasted with the lab::event::source::send_event
 * method to carry this event source identifier, and this way allowing every
 * listener to know which source have sent the event.
 */
#define SOURCE_MAKE_ID(_class) \
    static lab::event::source::id get_id() { \
        return typeid(_class).hash_code(); \
    }

/**
 * An event source is an object that will generate lab events. An event source
 * has some registred listeners (that are other objects) that will receive
 * every event sent by the source.
 */
class source {
  private:
    std::list<listener*> _listeners;

  public:
    typedef size_t id;

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
     * This method will auto-magically set the "source_id" of the event to
     * the right event source ID (this is the reason why this method is
     * virtual. Don't try to overload it else you know what you're doing).
     * @throw Throws exceptions thrown by listeners at event reception.
     */
    virtual void send_event(event event) throw(util::exception);

    void relay_event(event event) throw(util::exception);

    SOURCE_MAKE_ID(source)
};

}}

#endif

