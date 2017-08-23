#ifndef _shadow_renderer_screen_hpp_
#define _shadow_renderer_screen_hpp_

#include "lab/lab.hpp"
#include "renderer.hpp"
#include "point.hpp"

class screen : public lab::engine::screen {
  private:
    point _light_position;
    renderer _renderer;

  public:
    screen();

    ~screen();

    void start(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

    void update(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

    void stop(lab::engine::engine_interface* intf)
        throw(lab::util::exception);

    void notify(const lab::event::event& evt)
        throw(lab::util::exception);

};

#endif

