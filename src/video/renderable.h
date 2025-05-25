#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <stdint.h>
#include "shader.h"

typedef struct texture_params texture_params_t;
struct texture_params {
    uint32_t id;
    int32_t width, height, nr_components;
};

typedef struct renderable_2d_mesh Render2DMesh;
struct renderable_2d_mesh {
    uint32_t vao;
    uint32_t vbo;
    texture_params_t texture;
    Shader shader;
};

Render2DMesh create_2d_mesh(const float* mesh_data, int data_len, const char* vs, const char* fs, const char* tex);
Render2DMesh create_2d_mesh_with_texture(const float* mesh_data, int data_len, const char* vs, const char* fs, uint8_t* texture_data,  int tex_width, int tex_height, int nr_components);

void render_2d_mesh(Render2DMesh* mesh);

void release_2d_mesh(Render2DMesh* mesh);


#endif // RENDERABLE_H_
