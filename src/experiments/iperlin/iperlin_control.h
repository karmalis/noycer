#ifndef IPERLIN_CONTROL_H_
#define IPERLIN_CONTROL_H_

#include <stdint.h>
#include <cglm/cglm.h>

typedef struct noise_state noise_state;
void generate_noise(int width, int height, double depth, struct noise_state* noise, uint8_t* pixels);

void iperlin_control_setup(uint32_t width, uint32_t height);
void iperlin_control_update();
void iperlin_control_render(mat4 projection);
void iperlin_control_shutdown();

#endif // IPERLIN_CONTROL_H_
