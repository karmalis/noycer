#ifndef MODEL_H_
#define MODEL_H_

#include <stddef.h>
typedef struct model model_t;
struct model {
    float* vertices;
    size_t size;
};

model_t cube();
model_t plane();
model_t screen_quad();

#endif // MODEL_H_
