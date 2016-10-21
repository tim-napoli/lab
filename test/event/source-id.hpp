#include <string>
#include <cxxtest/TestSuite.h>
#include "lab/event/event.hpp"
#include "lab/event/source.hpp"

using namespace lab::event;
using namespace lab::util;

class alice : public listener {
  private:
    source::id _source_id;

  public:
    alice() {

    }

    ~alice() {

    }

    source::id get_source_id() const {
        return _source_id;
    }

    void notify(const event& evt) throw(exception) {
        _source_id = evt.get_source_id();
    }
};

class source_a : public source {
  public:
    SOURCE_MAKE_ID(source_a)
};

class source_b : public source {
  public:
    SOURCE_MAKE_ID(source_b)
};

class SourceIdTestSuite : public CxxTest::TestSuite
{
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testId() {
        alice  alice;
        source_a source_a;
        source_b source_b;

        event evt(0);

        source_a.register_listener(&alice);
        source_b.register_listener(&alice);

        source_a.send_event(evt);
        TS_ASSERT(alice.get_source_id() == source_a::get_id());

        source_b.send_event(evt);
        TS_ASSERT(alice.get_source_id() == source_b::get_id());
    }
};


