/**
 * @module      lab::gfx::program
 * @brief       Rendering program.
 * @author      Timothée Napoli
 * @license     WTFPL
 * @copyright   Timothée Napoli (c) 2017
 */
#ifndef _lab_gfx_program_hpp_
#define _lab_gfx_program_hpp_

#include <glm/glm.hpp>
#include <GL/gl.h>
#include "lab/util/exception.hpp"

namespace lab { namespace gfx {

/**
 * Rendering program. It uses the standard OpenGL programmable pipeline.
 */
class program {
  private:
    GLuint _vertex_shader_id;
    GLuint _fragment_shader_id;
    GLuint _program_id;

    GLint get_uniform_location(const std::string& name) const
        throw(util::exception);

  public:
    program();

    program(GLuint vertex_shader_id, GLuint fragment_shader_id,
             GLuint program_id);

    ~program();

    /**
     * Set this program as current rendering program.
     */
    void use() const;

    void set_uniform_mat4(const std::string& name, const glm::mat4* mat)
        throw(util::exception);

    void set_uniform_sampler2d(const std::string& name, GLuint id)
        throw(util::exception);

    void set_uniform_vec3(const std::string& name, glm::vec3 v)
        throw(util::exception);

    void set_uniform_float(const std::string& name, float v)
        throw(util::exception);

    /**
     * Destroy program data.
     */
    void destroy();

    /**
     * Load a program from given vertex and fragment shaders paths.
     * @throw util::exception if a shader is invalid.
     */
    static program load(const std::string& vertex_path,
                        const std::string& fragment_path)
        throw(lab::util::exception);
};

}}

#endif

