/**
 * @module      lab::gfx::draw_pass
 * @brief       The rendering draw pass module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_draw_pass_hpp_
#define _lab_gfx_draw_pass_hpp_

#include <list>
#include "lab/gfx/pass.hpp"
#include "lab/gfx/drawable.hpp"

namespace lab { namespace gfx {

/**
 * A rendering draw pass.
 */
class draw_pass : public pass {
  private:
    std::list<drawable*> _waiting_queue;

  public:
    draw_pass(program& prg, framebuffer* output);

    virtual ~draw_pass();

    void add_drawable(drawable* d);

    void render() throw(util::exception);
};

}}

#endif


