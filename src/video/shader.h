#ifndef SHADER_H_
#define SHADER_H_

#include <stdint.h>
#include <cglm/cglm.h>

typedef struct Shader Shader;
struct Shader {
    uint32_t vertex_shader;
    uint32_t fragment_shader;
    uint32_t program;
};

void shader_enable(Shader* shader);
void shader_disable(Shader* shader);

void shader_uniform1i(Shader* shader, const char* name, int value);
void shader_uniform_matrix4fv(Shader* shader, const char* name, mat4 mat);
void shader_uniform3fv(Shader* shader, const char* name, vec3 val);

void check_errors();

Shader create_shader(const char* vs, const char* fs);
void release_shader(Shader* shader);


#endif // SHADER_H_
