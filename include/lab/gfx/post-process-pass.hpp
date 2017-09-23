/**
 * @module      lab::gfx::post_process_pass
 * @brief       The rendering post process pass module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_post_process_pass_hpp_
#define _lab_gfx_post_process_pass_hpp_

#include <map>
#include "lab/math/box.hpp"
#include "lab/gfx/pass.hpp"
#include "lab/gfx/vertex-buffer.hpp"

namespace lab { namespace gfx {

/**
 * A rendering post_process pass.
 * This type of pass is used to do some post-processing of previous
 * framebuffers.
 *
 * It takes in input a set of framebuffers, and, when rendering, use every
 * of its framebuffers textures as GLSL sampler2D.
 * The framebuffers textures are bound to the program's uniforms using the
 * same uniform's name than the input framebuffer name.
 * For exemple, if a post-process pass has for input a framebuffer named
 * "canvas", its associated shader program must have a sampler2D uniform
 * named "canvas".
 *
 * The other important value of these passes is the viewport value, that
 * determines the input vertexes that are passed to the shader program.
 */
class post_process_pass : public pass {
  private:
    std::map<std::string, const framebuffer*> _input_fbs;
    vertex_buffer _vertices;

  public:
    post_process_pass(program& prg, framebuffer* output,
                      math::box viewport);

    virtual ~post_process_pass();

    void add_input(const std::string& name, const framebuffer* fb);

    void render() throw(util::exception);
};

}}

#endif


