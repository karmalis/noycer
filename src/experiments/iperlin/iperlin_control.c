#include <glad/gl.h>
#include <stdint.h>

#include "iperlin_control.h"

#include "../../base/base.h"
#include "iperlin.h"
#include "../../video/model.h"
#include "../../video/renderable.h"

#define DEFAULT_NOISE_OCTAVES 8
#define DEFAULT_NOISE_PER 0.75
#define DEFAULT_NOISE_BASE_FREQ 0.00095
#define DEFAULT_NOISE_BASE_AMP 0.5

typedef struct noise_state NoiseState;
struct noise_state {
    int octaves;
    double per;
    double bfreq;
    double bamp;
    double depth;
};

typedef struct render_params RenderParams;
struct render_params {
    Render2DMesh noise_mesh;
};

NoiseState noise_s = {
.octaves = DEFAULT_NOISE_OCTAVES,
.per = DEFAULT_NOISE_PER,
.bfreq = DEFAULT_NOISE_BASE_FREQ,
.bamp = DEFAULT_NOISE_BASE_AMP,
.depth = 0.0
};

RenderParams r_params = {
.noise_mesh = 0,
};

void generate_noise(int width, int height, double depth, struct noise_state* noise, uint8_t* pixels) {
 for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t index = (size_t)(y*width + x) * 3;
            double n_v = octave_iperlin_at(
                (double) x, (double) y, depth,
                noise->octaves,
                noise->per,
                noise->bfreq,
                noise->bamp);
            uint8_t val = (uint8_t)((n_v * 0.5 + 0.5) * 255.0);
            pixels[index] = val;
            pixels[index+1] = val;
            pixels[index+2] = val;
        }
    }
}

void iperlin_control_setup(uint32_t width, uint32_t height) {
   uint8_t* pixels = (uint8_t*) malloc((size_t)(width * height * 3));
   generate_noise(width, height, noise_s.depth, &noise_s, pixels);

   Model model = model_sprite();

   r_params.noise_mesh = create_2d_mesh_with_texture(
       model.vertices,
       model.size,
       "shaders/sprite.vert",
       "shaders/sprite.frag",
       pixels,
       width,
       height,
       3);
}

void iperlin_control_update() {

}

void iperlin_control_render(mat4 projection) {
    mat4 model;
    glm_mat4_identity(model);
    //vec3 translate_pos = { (float)g_curr_width / 2.0f, (float)g_curr_height / 2.0f, 0.0f };
    vec3 translate_pos = { 0.0, 0.0 };

    glm_translate(model, translate_pos);
    vec3 scale_factor = { (float)r_params.noise_mesh.texture.width, (float)r_params.noise_mesh.texture.height, 1.0f };
    //vec3 scale_factor = { g_curr_width, g_curr_height };

    glm_scale(model, scale_factor);

    glUseProgram(r_params.noise_mesh.shader.program);
    shader_uniform_matrix4fv(&r_params.noise_mesh.shader, "model", model);
    shader_uniform_matrix4fv(&r_params.noise_mesh.shader, "projection", projection);

    vec3 col;
    col[0] = 1.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    shader_uniform3fv(&r_params.noise_mesh.shader, "spriteColor", col);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, r_params.noise_mesh.texture.id);
    shader_uniform1i(&r_params.noise_mesh.shader, "spriteTexture", 0);

    glDisable(GL_STENCIL_TEST); // Explicitly disable stencil test
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    r2d_mesh_render(&r_params.noise_mesh);


}

void iperlin_control_shutdown() {
  r2d_mesh_release(&r_params.noise_mesh);
}
