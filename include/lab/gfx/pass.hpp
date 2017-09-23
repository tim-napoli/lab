/**
 * @module      lab::gfx::pass
 * @brief       The rendering pass module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_pass_hpp_
#define _lab_gfx_pass_hpp_

#include <string>
#include "lab/util/exception.hpp"
#include "lab/gfx/program.hpp"
#include "lab/gfx/framebuffer.hpp"

namespace lab { namespace gfx {

/**
 * A rendering pass.
 * Basically, a pass can be viewed as a part of the rendering process
 * taking in input either game scene objects or previous passes result
 * (framebuffers), and produces a new result from that, using the given
 * shader program.
 * The first type of pass is called a draw pass, and the second a post-process
 * pass.
 *
 * Shader programs used in passes must fit specific requirements in function
 * of the pass type. See documentation of lab::gfx::draw_pass and
 * lab::gfx::post_process_pass for more details.
 *
 * The pass owns its output framebuffer. Don't try to delete it from anywhere
 * else.
 * If the output framebuffer reference is NULL, then this pass targets the
 * screen.
 */
class pass {
  private:
    program&     _program;
    framebuffer* _output_fb;

  public:
    pass(program& prg, framebuffer* output);

    virtual ~pass();

    const framebuffer* get_output() const {return _output_fb;}

    program& get_program() {return _program;}

    /**
     * Pass rendering method.
     * @throw an exception if something goes wrong.
     */
    virtual void render() throw(util::exception) = 0;
};

}}

#endif

