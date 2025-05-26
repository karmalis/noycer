#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <stdint.h>

typedef struct Framebuffer Framebuffer;
struct Framebuffer {
    uint32_t fbo; // framebuffer
    uint32_t texture_color_buffer;
    uint32_t rbo; // renderbuffer
    uint32_t width;
    uint32_t height;
};

Framebuffer framebuffer_create(uint32_t width, uint32_t height);

void framebuffer_enable(Framebuffer* buffer);
void framebuffer_disable(Framebuffer* buffer);

void framebuffer_release(Framebuffer* buffer);

#endif // FRAMEBUFFER_H_
