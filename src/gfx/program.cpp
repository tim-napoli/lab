#include <fstream>
#include <GL/glew.h>
#include "lab/gfx/program.hpp"

namespace lab { namespace gfx {

program::program()
    : _vertex_shader_id(0)
    , _fragment_shader_id(0)
    , _program_id(0)
{

}

program::program(GLuint vertex_shader_id, GLuint fragment_shader_id,
                 GLuint program_id)
    : _vertex_shader_id(vertex_shader_id)
    , _fragment_shader_id(fragment_shader_id)
    , _program_id(program_id)
{

}

program::~program() {
}

void program::use() const {
    glUseProgram(_program_id);
}

void program::destroy() {
    glDeleteShader(_vertex_shader_id);
    glDeleteShader(_fragment_shader_id);
    glDeleteProgram(_program_id);
}

GLint program::get_uniform_location(const std::string& name) const
        throw(util::exception)
{
    GLint loc = glGetUniformLocation(_program_id, name.c_str());
    if (loc < 0) {
        throw util::exception::build_formatted(
            "Requesting unavailable uniform '{}' in shader program",
            name
        );
    }
    return loc;
}

void program::set_uniform_mat4(const std::string& name,
                               const glm::mat4* mat)
        throw(util::exception)
{
    use();
    GLint loc = get_uniform_location(name);
    glUniformMatrix4fv(loc, 1, false, (const GLfloat*)mat);
}

void program::set_uniform_mat3(const std::string& name,
                               const glm::mat3* mat)
        throw(util::exception)
{
    use();
    GLint loc = get_uniform_location(name);
    glUniformMatrix3fv(loc, 1, false, (const GLfloat*)mat);
}

void program::set_uniform_sampler2d(const std::string& name,
                                    GLuint id)
        throw(util::exception)
{
    use();
    GLint loc = get_uniform_location(name);
    glUniform1i(loc, id);
}

void program::set_uniform_vec3(const std::string& name,
                               glm::vec3 v)
        throw(util::exception)
{
    use();
    GLint loc = get_uniform_location(name);
    glUniform3fv(loc, 1, (const GLfloat*)&v);
}

void program::set_uniform_float(const std::string& name, float v)
    throw(util::exception)
{
    use();
    GLint loc = get_uniform_location(name);
    glUniform1f(loc, v);
}

static void load_shader_source(GLuint shader, std::istream& input)
{
    input.seekg(0, input.end);
    size_t input_size = input.tellg();
    input.seekg(0, input.beg);

    char* source = new char[input_size + 1];
    input.read(source, input_size);
    source[input_size] = '\0';
    glShaderSource(shader, 1, (const char**)&source, NULL);
    delete[] source;
}

static void load_and_compile_shader(GLuint shader, const std::string& path)
    throw(util::exception)
{
    GLint shader_status;
    GLint shader_info_len;

    std::ifstream file(path, std::ifstream::in);
    if (!file.is_open()) {
        throw util::exception::build_formatted(
            "Cannot open shader file '{}'",
            path
        );
    }
    load_shader_source(shader, file);

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
    if (shader_status == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shader_info_len);
        char* info_log = new char[shader_info_len];
        glGetShaderInfoLog(shader, shader_info_len, NULL, info_log);
        util::exception ex = util::exception::build_formatted(
            "Shader '{}' compilation failure: {}",
            path, info_log
        );
        delete[] info_log;
        throw ex;
    }
}

program program::load(const std::string& vertex_path,
                      const std::string& fragment_path)
    throw(util::exception)
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    load_and_compile_shader(vertex_shader_id, vertex_path);
    load_and_compile_shader(fragment_shader_id, fragment_path);

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    GLint link_status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        GLint info_log_len;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);
        char* info_log = new char[info_log_len];
        glGetProgramInfoLog(program_id, info_log_len, NULL, info_log);
        util::exception ex = util::exception::build_formatted(
            "Program compilation failure: {}",
            info_log
        );
        delete[] info_log;
        throw ex;
    }

    return program(vertex_shader_id, fragment_shader_id, program_id);
}

}}

