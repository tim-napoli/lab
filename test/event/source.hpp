#include <string>
#include <cxxtest/TestSuite.h>
#include "lab/event/event.hpp"
#include "lab/event/source.hpp"

using namespace lab::event;
using namespace lab::util;

class alice : public listener {
  private:
    std::string _message;

  public:
    alice() {

    }

    ~alice() {

    }

    const std::string& get_message() const {
        return _message;
    }

    void notify(const event& evt) throw(exception) {
        _message = evt.get_value("message").get_value<std::string>();
    }
};

class SourceTestSuite : public CxxTest::TestSuite, public source
{
  public:
    SourceTestSuite() : CxxTest::TestSuite(), source() {

    }

    void setUp() {

    }

    void tearDown() {

    }

    void testEvent() {
        alice alice_a;
        alice alice_b;

        register_listener(&alice_a);
        register_listener(&alice_b);

        event evt(0);
        evt.set_value("message", value::build<std::string>("blectre"));
        send_event(evt);

        TS_ASSERT(alice_a.get_message() == "blectre");
        TS_ASSERT(alice_b.get_message() == "blectre");

        unregister_listener(&alice_b);
        evt.set_value("message", value::build<std::string>("miaou"));
        send_event(evt);

        TS_ASSERT(alice_a.get_message() == "miaou");
        TS_ASSERT(alice_b.get_message() == "blectre");

        evt = event(42);
        evt.set_value("msg", value::build<std::string>("blectre"));
        TS_ASSERT_THROWS(send_event(evt), exception);
    }
};

