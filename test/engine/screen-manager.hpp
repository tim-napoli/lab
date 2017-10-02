#include <unistd.h>
#include <thread>
#include <cxxtest/TestSuite.h>
#include "mocks/gl.hpp"
#include "mocks/glfw.hpp"

#define private public  // Wow, that's tricky ?
#include "lab/engine/screen-manager.hpp"

using namespace lab;

// test source ----------------------------------------------------------------

class test_source : public event::source {
  public:
    enum events : event::event::type {
        hello,
    };

    SOURCE_MAKE_ID(test_source)
};

// test screen ----------------------------------------------------------------

class test_screen : public engine::screen {
  private:
    bool _received_event;
    bool _started;

  public:
    test_screen() : _received_event(false)
                  , _started(false)
    { }

    ~test_screen() { }

    void start(engine::engine_interface* intf) throw(util::exception) {
        _started = true;
    }

    void stop(engine::engine_interface* intf) throw(util::exception) {
    }

    void update(engine::engine_interface* intf) throw(util::exception) {

    }

    void notify(const event::event& evt) throw(util::exception) {
        if (evt.get_source_id() == test_source::get_id()) {
            TS_ASSERT(evt.get_type() == test_source::events::hello);
            _received_event = true;
        }
    }

    bool is_started() const {return _started;}

    bool received_event() const {return _received_event;}
};

// tests ----------------------------------------------------------------------

class ScreenManagerTestSuite : public CxxTest::TestSuite {
  public:
    void setUp() {

    }

    void tearDown() {

    }

    void testEvents() {
    }

    void testScreenEvent() {
        engine::screen_manager manager;
        test_source source;
        source.register_listener(&manager);

        test_screen* scr_a = new test_screen();
        manager.push_screen(scr_a);
        TS_ASSERT(!scr_a->is_started());
        manager.update(NULL);
        TS_ASSERT(scr_a->is_started());

        test_screen* scr_b = new test_screen();
        manager.push_screen(scr_b);
        manager.update(NULL);

        source.send_event(event::event(test_source::events::hello));
        TS_ASSERT(scr_b->received_event());
        TS_ASSERT(!scr_a->received_event());

        TS_ASSERT(manager._screens.front().state == engine::paused);
        TS_ASSERT(scr_b->is_started());

        manager.pop_screen(NULL);
        TS_ASSERT(manager._screens.size() == 1);
        manager.update(NULL);
        source.send_event(event::event(test_source::events::hello));
        TS_ASSERT(scr_a->received_event());
        TS_ASSERT(scr_a->is_started());
    }
};

// ----------------------------------------------------------------------------

