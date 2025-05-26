#ifndef MODEL_H_
#define MODEL_H_

#include <stddef.h>
typedef struct model Model;
struct model {
    float* vertices;
    size_t size;
};

Model model_cube();
Model model_plane();
Model model_screen_quad();
Model model_sprite();

#endif // MODEL_H_
