/**
 * @module      lab::event::event
 * @brief       Base event class for the Lab Engine.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 *
 * XXX This file is used as the module file and as the event class file.
 */
#ifndef _lab_event_event_hpp_
#define _lab_event_event_hpp_

#include <map>
#include <string>
#include "lab/util/value.hpp"
#include "lab/util/exception.hpp"

namespace lab { namespace event {

/**
 * An event is a simple object mapping some names (string) with their
 * value (util::value).
 * An event has a type, an integer used to distinct their roles.
 * It is a super-generic way to allow multiple components with no knowledge
 * each from other to communicate. Be careful about performances when using
 * events to make multiple components to talk, because it is slow.
 */
class event {
  private:
    size_t                             _source_id;
    int                                _type;
    std::map<std::string, util::value> _values;

    void set_source_id(size_t id) {_source_id = id;}

  public:
    event(int type = -1);

    ~event();

    int get_type() const {return _type;}

    size_t get_source_id() const {return _source_id;}

    void set_type(int type) {_type = type;}

    void set_value(const std::string& name, const util::value& value);

    const util::value& get_value(const std::string& name) const
        throw(util::exception);

    friend class source;
};

}}

#endif

