#include <glad/gl.h>
#include <stdint.h>

#include "iperlin_control.h"

#include "../base/base.h"
#include "../iperlin.h"
#include "../video/model.h"
#include "../video/renderable.h"

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
    Model model
};

NoiseState noise_s = {
.octaves = DEFAULT_NOISE_OCTAVES,
.per = DEFAULT_NOISE_PER,
.bfreq = DEFAULT_NOISE_BASE_FREQ,
.bamp = DEFAULT_NOISE_BASE_AMP,
.depth = 0.0
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

   Render2DMesh mesh = create_2d_mesh_with_texture(
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

void iperlin_control_render() {

}

void iperlin_control_shutdown() {

}
