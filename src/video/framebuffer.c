#include <glad/gl.h>

#include "../base/base.h"
#include "framebuffer.h"

Framebuffer framebuffer_create(uint32_t width, uint32_t height) {
    Framebuffer buffer = {0};

    glGenFramebuffers(1, &buffer.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer.fbo);

    glGenTextures(1, &buffer.texture_color_buffer);
    glBindTexture(GL_TEXTURE_2D, buffer.texture_color_buffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texture_color_buffer, 0);

    glGenRenderbuffers(1, &buffer.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, buffer.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        error_print("Framebuffer incomplete");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    buffer.width = width;
    buffer.height = height;

    return buffer;
}

void framebuffer_enable(Framebuffer* buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);
    glEnable(GL_DEPTH_TEST);
}

void framebuffer_disable(Framebuffer* buffer) {
    unused(buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer_release(Framebuffer* buffer) {
    glDeleteTextures(1, &buffer->texture_color_buffer);
    glDeleteRenderbuffers(1, &buffer->rbo);
    glDeleteFramebuffers(1, &buffer->fbo);
}
