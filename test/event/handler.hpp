#include <string>
#include <cxxtest/TestSuite.h>
#include "lab/event/event.hpp"
#include "lab/event/source.hpp"
#include "lab/event/handler.hpp"

using namespace lab::event;
using namespace lab::util;

class source_a : public source {
  public:
    static const event::type set_one = 1;
    static const event::type add_two = 2;

    SOURCE_MAKE_ID(source_a)
};

class source_b : public source {
  public:
    static const event::type mul_by_three = 3;
    static const event::type div_by_zero = 4;

    SOURCE_MAKE_ID(source_b)
};

class alice : public listener {
  private:
    handler _handler;
    int     _number;

  public:
    alice() : _handler() {
        _handler.add_handler(source_a::get_id(), source_a::set_one,
            [this](const event& evt) {
                _number = 1;
            }
        );
        _handler.add_handler(source_a::get_id(), source_a::add_two,
            [this](const event& evt) {
                _number += 2;
            }
        );
        _handler.add_handler(source_b::get_id(), source_b::mul_by_three,
            [this](const event& evt) {
                _number *= 3;
            }
        );
        _handler.add_handler(source_b::get_id(), source_b::div_by_zero,
            [this](const event& evt) {
                throw exception("division by zero");
            }
        );
    }

    ~alice() {

    }

    int get_number() const {return _number;}

    void notify(const event& evt) throw(exception) {
        _handler.handle(evt);
    }
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

        source_a.register_listener(&alice);
        source_b.register_listener(&alice);

        source_a.send_event(event(source_a::set_one));
        TS_ASSERT(alice.get_number() == 1);

        source_a.send_event(event(source_a::add_two));
        TS_ASSERT(alice.get_number() == 3);

        source_a.send_event(event(source_b::mul_by_three));
        TS_ASSERT(alice.get_number() == 3);

        source_b.send_event(event(source_b::mul_by_three));
        TS_ASSERT(alice.get_number() == 9);

        TS_ASSERT_THROWS(source_b.send_event(event(source_b::div_by_zero)),
                         exception);
    }
};


