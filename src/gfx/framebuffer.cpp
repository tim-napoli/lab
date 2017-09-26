#include <GL/glew.h>
#include "lab/gfx/framebuffer.hpp"

namespace lab { namespace gfx {

framebuffer::framebuffer() : _fbo_id(0)
                           , _texture_id(0)
                           , _width(0)
                           , _height(0)
{

}

framebuffer::framebuffer(int width, int height)
    throw(util::exception)
        : _fbo_id(0)
        , _texture_id(0)
        , _width(width)
        , _height(height)
{
    glGenFramebuffers(1, &_fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo_id);

    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _texture_id, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw util::exception("Invalid framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

framebuffer::~framebuffer() {

}

void framebuffer::destroy() {
    glDeleteTextures(1, &_texture_id);
    _texture_id = 0;
    glDeleteFramebuffers(1, &_fbo_id);
}

void framebuffer::bind_texture(GLuint unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

}}

