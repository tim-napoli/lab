/**
 * @module      lab::gfx::animation
 * @brief       The engine gfx animation class.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2016
 */
#ifndef _lab_gfx_animation_hpp_
#define _lab_gfx_animation_hpp_

#include "lab/gfx/image.hpp"

namespace lab { namespace gfx {

class animation_frame {
  private:
    image        _image;
    unsigned int _tick_duration;

  public:
    animation_frame(image img, unsigned int tick_duration);

    ~animation_frame();

    unsigned int get_tick_duration() const {return _tick_duration;}

    void draw() const;

    void destroy();
};

class animation {
  private:
    std::vector<animation_frame> _frames;
    unsigned int                 _current_frame;
    unsigned int                 _counter;

  public:
    animation();
    animation(const std::vector<animation_frame>& frames);

    ~animation();

    void update();

    void draw() const;

    void destroy();
};

}}

#endif

