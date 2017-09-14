#include "lab/gfx/texture.hpp"
#include <SOIL/SOIL.h>

namespace lab { namespace gfx {

texture::texture() : _texture_id(0)
{

}

texture::texture(GLuint tex_id) : _texture_id(tex_id)
{

}

texture::~texture() {

}

glm::vec2 texture::get_dimensions() const {
    int w, h;
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

    return glm::vec2((float)w, (float)h);
}

void texture::destroy() {
    glDeleteTextures(1, &_texture_id);
}

texture texture::load(const std::string& path) throw(util::exception) {
    GLuint tex = SOIL_load_OGL_texture(
        path.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (tex == 0) {
        throw util::exception::build_formatted(
            "cannot load texture '{}'", path
        );
    }
    return texture(tex);
}

}}

