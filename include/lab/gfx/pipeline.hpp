/**
 * @module      lab::gfx::pipeline
 * @brief       The rendering pipeline module.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_pipeline_hpp_
#define _lab_gfx_pipeline_hpp_

#include <map>
#include <memory>
#include "lab/util/exception.hpp"
#include "lab/gfx/renderer.hpp"
#include "lab/gfx/framebuffer.hpp"
#include "lab/gfx/draw-pass.hpp"
#include "lab/gfx/post-process-pass.hpp"

namespace lab { namespace gfx {

/**
 * A pipeline is a specialized renderer that add the notion of "rendering pass"
 * to the renderer in order to ease rendering process.
 *
 * A typical pipeline have at least one draw pass, charged to draw scene
 * objects to a framebuffer, and have a final pass that write intermediate
 * framebuffers on the screen.
 * Notate that the draw pass can target the screen directly.
 *
 * Pass rendering is done in the order we give passes to it. Don't put a pass
 * 'A' depending on the result of a pass 'B' before the pass B in the pipeline,
 * or an unefined behavior could happen.
 * However, you should notate that drawing passes are ever used before
 * post-process passes.
 *
 * Here is a typical exemple of how to programmaticaly do a simple rendering
 * pipeline:
 * @code
 *      // Pipeline declaration
 *      pipeline my_pipeline;
 *
 *      // Passes declaration.
 *      draw_pass* my_image_drawing_pass = new draw_pass(
 *          my_image_program, framebuffer(WIDTH, HEIGHT)
 *      );
 *      post_process_pass* my_blur_effect_pass = new post_process_pass(
 *          my_blur_program,
            framebuffer(WIDTH, HEIGHT),
 *          math::box(0, WIDTH, 0, HEIGHT)
 *      );
 *      my_blur_effect_pass->add_input(
 *          "images_fb",
 *          my_image_drawing_pass->get_output()
 *      );
 *
 *      // Add passes to the pipeline
 *      my_pipeline.add_draw_pass("images_pass", my_image_drawing_pass);
 *      my_pipeline.add_post_process_pass("blur_pass", my_blur_effect_pass);
 *
 *      // Next, we add an image to the image pass, and we render it.
 *      my_image_drawing_pass->add_drawable(some_image_drawable);
 *
 *      my_pipeline.render();
 * @endcode
 *
 * If a pass targets the screen instead of a specific framebuffer, its
 * output framebuffer must be the string "screen".
 *
 * The pipeline owns the passes we give to it, don't try to delete them from
 * anywhere else.
 *
 * @see lab::gfx::renderer
 * @see lab::gfx::drawable
 * @see lab::gfx::draw_pass
 * @see lab::gfx::post_process_pass
 */
class pipeline : public renderer {
  private:
    std::map<std::string, draw_pass*> _draw_passes;
    std::map<std::string, post_process_pass*> _post_process_passes;

    /*
     * These arrays will contain every passes, and are used to know the
     * passes rendering order.
     */
    std::vector<draw_pass*> _draw_passes_order;
    std::vector<post_process_pass*> _post_process_passes_order;

    void set_output_framebuffer(pass* p);

    std::string get_name_of_draw_pass(draw_pass* p) const;
    std::string get_name_of_post_process_pass(post_process_pass* p) const;

  public:
    pipeline(int virtual_width, int virtual_height);

    ~pipeline();

    /**
     * Add a draw pass to the pipeline.
     * @throw if there is already a draw pass called 'name'.
     */
    void add_draw_pass(const std::string& name, draw_pass* p)
        throw(util::exception);

    /**
     * Returns the draw pass called 'name'.
     * @throw if the pipeline doesn't contain any pass called 'name'.
     */
    draw_pass* get_draw_pass(const std::string& name)
        throw(util::exception);

    /**
     * Add a post-process pass to the pipeline.
     * @throw if there is already a post-process pass called 'name'.
     */
    void add_post_process_pass(const std::string& name, post_process_pass* p)
        throw(util::exception);

    /**
     * Render the pipeline.
     * @throw if something goes wrong during rendering, for exemple if we
     *        try to set a uniform to a program shader that doesn't contain
     *        such uniform.
     */
    virtual void render() throw(util::exception);
};

}}

#endif

