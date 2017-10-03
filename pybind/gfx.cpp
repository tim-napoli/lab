#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "glm-cast.hpp"
#include "lab/gfx/renderer.hpp"
#include "lab/gfx/drawable.hpp"
#include "lab/gfx/framebuffer.hpp"
#include "lab/gfx/program.hpp"
#include "lab/gfx/animation.hpp"
#include "lab/gfx/pass.hpp"
#include "lab/gfx/draw-pass.hpp"
#include "lab/gfx/post-process-pass.hpp"
#include "lab/gfx/pipeline.hpp"
#include "lab/gfx/light.hpp"
#include "lab/gfx/texture.hpp"
#include "lab/gfx/image.hpp"
#include "lab/gfx/animation.hpp"
#include "lab/gfx/image-drawable.hpp"

using namespace lab;

namespace py = pybind11;

class py_drawable : public gfx::drawable {
  public:
    py_drawable(glm::mat3 mv_matrix)
        : drawable(mv_matrix)
    {

    }

    virtual ~py_drawable() { }

    void bind(gfx::program& shader) const throw(util::exception) override {
        PYBIND11_OVERLOAD(
            void,
            gfx::drawable,
            bind,
            shader
        );
    }

    void draw() const override {
        PYBIND11_OVERLOAD_PURE(
            void,
            gfx::drawable,
            draw
        );
    }
};

class py_pass : public gfx::pass {
  public:
    py_pass(gfx::program& shader, gfx::framebuffer* output)
        : gfx::pass(shader, output)
    {

    }

    virtual ~py_pass() { }

    void render() throw(util::exception) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            gfx::pass,
            render
        )
    }
};

class py_pipeline : public gfx::pipeline {
  public:
    py_pipeline(int vwidth, int vheight) : gfx::pipeline(vwidth, vheight) { }

    virtual ~py_pipeline() { }

    void render() throw(util::exception) override {
        PYBIND11_OVERLOAD(
            void,
            gfx::pipeline,
            render
        );
    }
};

PYBIND11_MODULE(gfx, m) {
    m.doc() = "lab::gfx python binding.";

    py::class_<gfx::renderer> gfx_renderer(m, "renderer");
    gfx_renderer
        .def(py::init<int, int>())
        .def("resize", &gfx::renderer::resize)
        .def(
            "get_real_to_virt_matrix",
            &gfx::renderer::get_real_to_virt_matrix
        )
        .def(
            "get_projection_matrix_ptr",
            &gfx::renderer::get_projection_matrix_ptr
        )
        .def(
            "use_framebuffer",
            (void (gfx::renderer::*)(const gfx::framebuffer&))
            &gfx::renderer::use_framebuffer
        )
        .def("use_screen", &gfx::renderer::use_screen)
        .def("set_windows_dimensions", &gfx::renderer::set_windows_dimensions)
        ;

    py::class_<gfx::framebuffer>(m, "framebuffer")
        .def(py::init<int, int>())
        .def("get_texture_id",  &gfx::framebuffer::get_texture_id)
        .def("get_fbo_id",      &gfx::framebuffer::get_fbo_id)
        .def("destroy",         &gfx::framebuffer::destroy)
        .def("get_width",       &gfx::framebuffer::get_width)
        .def("get_height",      &gfx::framebuffer::get_height)
        .def("bind_texture",    &gfx::framebuffer::bind_texture)
        ;

    py::class_<gfx::program>(m, "program")
        .def("use",                     &gfx::program::use)
        .def("set_uniform_mat4",        &gfx::program::set_uniform_mat4)
        .def("set_uniform_mat3",        &gfx::program::set_uniform_mat3)
        .def("set_uniform_sampler2d",   &gfx::program::set_uniform_sampler2d)
        .def("set_uniform_vec3",        &gfx::program::set_uniform_vec3)
        .def("set_uniform_float",       &gfx::program::set_uniform_float)
        .def("destroy",                 &gfx::program::destroy)
        .def_static("load",             &gfx::program::load)
        ;

    py::class_<gfx::pass, py_pass> gfx_pass(m, "pass");
    gfx_pass
        .def(py::init<gfx::program&, gfx::framebuffer*>(),
             py::keep_alive<1, 3>())
        .def("get_output",  &gfx::pass::get_output,
             py::return_value_policy::reference
        )
        .def("get_program", &gfx::pass::get_program,
             py::return_value_policy::reference
        )
        .def("render",      &gfx::pass::render)
        ;

    py::class_<gfx::draw_pass>(m, "draw_pass", gfx_pass)
        .def(py::init<gfx::program&, gfx::framebuffer*>(),
             py::keep_alive<1, 3>())
        .def("add_drawable",    &gfx::draw_pass::add_drawable)
        .def("render",          &gfx::draw_pass::render)
        ;

    py::class_<gfx::post_process_pass>(m, "post_process_pass", gfx_pass)
        .def(py::init<gfx::program&, gfx::framebuffer*, math::box>(),
             py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("add_input",    &gfx::post_process_pass::add_input,
             py::keep_alive<1, 3>())
        .def("render",       &gfx::post_process_pass::render)
        ;

    py::class_<gfx::drawable, py_drawable> gfx_drawable(m, "drawable");
    gfx_drawable
        .def(py::init<glm::mat3>())
        .def("bind", &gfx::drawable::bind)
        .def("draw", &gfx::drawable::draw)
        ;

    py::class_<gfx::pipeline, py_pipeline>(m, "pipeline", gfx_renderer)
        .def(py::init<int, int>())
        .def("add_draw_pass",           &gfx::pipeline::add_draw_pass,
             py::keep_alive<1, 3>())
        .def("get_draw_pass",           &gfx::pipeline::get_draw_pass,
             py::return_value_policy::reference)
        .def("add_post_process_pass",   &gfx::pipeline::add_post_process_pass,
             py::keep_alive<1, 3>())
        .def("render",                  &gfx::pipeline::render)
        ;

    py::class_<gfx::light>(m, "light")
        .def(py::init())
        .def(py::init<glm::vec3, float, glm::vec3>())
        .def("get_position",    &gfx::light::get_position)
        .def("get_radius",      &gfx::light::get_radius)
        .def("get_color",       &gfx::light::get_color)
        .def("set_position",    &gfx::light::set_position)
        ;

    py::class_<gfx::texture>(m, "texture")
        .def(py::init<GLuint>())
        .def("get_texture_id",  &gfx::texture::get_texture_id)
        .def("get_dimensions",  &gfx::texture::get_dimensions)
        .def("destroy",         &gfx::texture::destroy)
        .def_static("load",     &gfx::texture::load)
        ;

    py::class_<gfx::animation>(m, "animation")
        .def(py::init())
        .def("update", &gfx::animation::update)
        .def("draw", &gfx::animation::draw)
        .def("destroy", &gfx::animation::destroy)
        ;

    py::class_<gfx::image>(m, "image")
        .def(py::init())
        .def(py::init<const std::vector<gfx::texture>, glm::vec2>())
        .def(py::init<const std::vector<gfx::texture>, glm::vec2, glm::vec2>())
        .def("get_textures", &gfx::image::get_textures)
        .def("draw",         &gfx::image::draw)
        .def("destroy",      &gfx::image::destroy)
        ;

    py::class_<gfx::image_drawable>(m, "image_drawable", gfx_drawable)
        .def(py::init())
        .def(
            py::init<glm::mat3, const gfx::image*, std::vector<std::string>>(),
            py::keep_alive<1, 3>()
        )
        .def("bind", &gfx::image_drawable::bind)
        .def("draw", &gfx::image_drawable::draw)
        ;
}

