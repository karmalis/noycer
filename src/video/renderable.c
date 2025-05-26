#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../base/base.h"
#include "renderable.h"


texture_params_t load_texture_with_data(uint8_t* data, int width, int height, int nr_components) {
    texture_params_t texture = {0};
    uint32_t id;
    glGenTextures(1, &id);
    GLenum format;
    if (nr_components == 1) {
        format = GL_RED;
    } else if (nr_components == 3) {
        format = GL_RGB;
    } else if (nr_components == 4) {
        format = GL_RGBA;
    } else {
        error_print("Invalid number of texture components");
        stbi_image_free(data);
        return texture;
    }

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

   glGenerateMipmap(GL_TEXTURE_2D);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//lib64/    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    texture.id = id;
    texture.width = width;
    texture.height = height;
    texture.nr_components = nr_components;

    return texture;
}

texture_params_t load_texture(const char* path) {
    texture_params_t texture = {0};
    uint32_t id;
    glGenTextures(1, &id);

    int32_t width, height, nr_components;
    uint8_t* data = stbi_load(path, &width, &height, &nr_components, 0);

    if (data) {
        texture = load_texture_with_data(data, width, height, nr_components);
    }

    return texture;
}

Render2DMesh create_2d_mesh_with_texture(const float* mesh_data, int data_len, const char* vs, const char* fs, uint8_t* texture_data, int tex_width, int tex_height, int nr_components) {
     Render2DMesh mesh = {0};

    uint32_t vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data_len, mesh_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mesh.vao = vao;
    mesh.vbo = vbo;

    mesh.shader = create_shader(vs, fs);

    mesh.texture = load_texture_with_data(texture_data, tex_width, tex_height, nr_components);

    return mesh;
}


Render2DMesh create_2d_mesh(const float* data, int data_len, const char* vs, const char* fs, const char* tex) {
    Render2DMesh mesh = {0};


    uint32_t vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data_len, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mesh.vao = vao;
    mesh.vbo = vbo;

    mesh.shader = create_shader(vs, fs);

    mesh.texture = load_texture(tex);

    return mesh;
}

void r2d_mesh_update_texture(Render2DMesh* mesh, uint8_t* pixels) {
    GLenum format = GL_GREEN;
    if (mesh->texture.nr_components == 1) {
        format = GL_RED;
    } else if (mesh->texture.nr_components == 3) {
        format = GL_RGB;
    } else if (mesh->texture.nr_components == 4) {
        format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, mesh->texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, mesh->texture.width, mesh->texture.height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void r2d_mesh_render(Render2DMesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void r2d_mesh_release(Render2DMesh* mesh) {
    glDeleteTextures(1, &mesh->texture.id);
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    release_shader(&mesh->shader);
}
