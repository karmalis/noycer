#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "../base/base.h"
#include "../base/util.h"

#include "shader.h"

bool check_shader_compilation(uint32_t shader) {
  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    error_print("Failed shader compilation: %s", info_log);
  }

  return success;
}

bool check_shader_program_link(uint32_t program) {
  int success;
  char info_log[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info_log);
    error_print("Failed shader compilation: %s", info_log);
  }

  return success;
}

void shader_enable(Shader* shader) {
    glUseProgram(shader->program);
}

void shader_disable(Shader* shader) {
    unused(shader);
    glUseProgram(0);
}

uint32_t load_and_compile_shader(const char* source, uint32_t type) {
    file_buff_t shader_source = read_full_file(source);
    uint32_t shader = glCreateShader(type);
    int32_t s_size = shader_source.length * sizeof(char);
    const GLchar* buffer = shader_source.buff;
    glShaderSource(shader, 1, &buffer, &s_size);
    glCompileShader(shader);
    if (!check_shader_compilation(shader)) {
        error_print("Could not compile shader %s", source);
    }

    free(shader_source.buff);

    return shader;
}

void shader_uniform1i(Shader* shader, const char* name, int value) {
    //shader_enable(shader);
    glUniform1i(glGetUniformLocation(shader->program, name), value);
    //shader_disable(shader);
}

void shader_uniform_matrix4fv(Shader* shader, const char* name, mat4 mat) {
  //shader_enable(shader);
  glUniformMatrix4fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, mat[0]);
  //shader_disable(shader);
}

void shader_uniform3fv(Shader* shader, const char* name, vec3 val) {
  //shader_enable(shader);
  glUniform3fv(glGetUniformLocation(shader->program, name), 1, val);
  //shader_disable(shader);
}

void check_errors() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    error_print("OpenGL Error: %d", err);
  }
}

Shader create_shader(const char* vs, const char* fs) {
    Shader shader = {0};
    uint32_t vertex_shader = load_and_compile_shader(vs, GL_VERTEX_SHADER);
    check_errors();
    uint32_t fragment_shader = load_and_compile_shader(fs, GL_FRAGMENT_SHADER);
    check_errors();
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    if (!check_shader_program_link(program)) {
        error_print("Could not link program for %s and %s", vs, fs);
    }

    shader.vertex_shader = vertex_shader;
    shader.fragment_shader = fragment_shader;
    shader.program = program;

    return shader;
}

void release_shader(Shader* shader) {
    glDeleteShader(shader->vertex_shader);
    glDeleteShader(shader->fragment_shader);
}
