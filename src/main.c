#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "video/shader.h"
#include "video/model.h"
#include "video/renderable.h"
#include "video/framebuffer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "base/base.h"
#include "base/util.h"

#include "dgui/dgui.h"

//#include "iperlin.h"
#include "experiments/iperlin/iperlin_control.h"

#define DEFAULT_NOISE_OCTAVES 8;
#define DEFAULT_NOISE_PER 0.75;
#define DEFAULT_NOISE_BASE_FREQ 0.00095;
#define DEFAULT_NOISE_BASE_AMP 0.5;

const GLuint WIDTH = 1200, HEIGHT = 800;

GLuint g_curr_width = WIDTH;
GLuint g_curr_height = HEIGHT;


const float vertices[] = {
    // Triangle 1
    -1.0f, -1.0f, 0.0f, // bottom left
     1.0f, -1.0f, 0.0f, // bottom right
     1.0f,  1.0f, 0.0f, // top right (new order)

    // Triangle 2
    -1.0f, -1.0f, 0.0f, // bottom left
     1.0f,  1.0f, 0.0f, // top right (new order)
    -1.0f,  1.0f, 0.0f  // top left
};


const uint32_t indices[] = {0, 1, 3, 1, 2, 3};

// Pixel noise state
struct noise_state {
    int octaves;
    double per;
    double bfreq;
    double bamp;
};


void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  unused(scancode);
  unused(mode);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  unused(window);
  g_curr_width = width;
  g_curr_height = height;
  glViewport(0, 0, width, height);
}

int main(void) {
  /// initialise glfw
  /// ---------------
  if (!glfwInit()) {
    fatal_msg("Could not initialise GLFW\n");
  }

  /// Set up opengl and window params
  /// -------------------------------
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE);

  /// Initialise window
  /// -----------------
  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Fun with noise", NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    fatal_msg("Could not initialise GLFW window\n");
  }
  glfwMakeContextCurrent(window);

  /// Input callback
  /// --------------
  glfwSetKeyCallback((window), key_callback);

  /// Initialise glad and GL functions
  /// --------------------------------
  int version = gladLoadGL(glfwGetProcAddress);
  if (!version) {
    fatal_msg("Could not initialise glad\n");
  }
  printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));

  /// Init viewport
  /// -------------
  glViewport(0, 0, g_curr_width, g_curr_height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  dgui dgui = {0};
  dgui_init(window, &dgui);

  // screen quad object init
  // -----------------------
  Model screen_quad_model = model_screen_quad();
  uint32_t screen_vao, screen_vbo;
  glGenVertexArrays(1, &screen_vao);
  glGenBuffers(1, &screen_vbo);
  glBindVertexArray(screen_vao);
  glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
  glBufferData(GL_ARRAY_BUFFER, screen_quad_model.size, screen_quad_model.vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glBindVertexArray(0);

  /// Framebuffer
  /// -----------
  Framebuffer fbuffer = framebuffer_create(g_curr_width, g_curr_height);

  uint8_t* noise_pixels = (uint8_t*) malloc((size_t)(g_curr_width * g_curr_height * 3));
  struct noise_state noise = {0};
  noise.octaves = DEFAULT_NOISE_OCTAVES;
  noise.per = DEFAULT_NOISE_PER;
  noise.bfreq = DEFAULT_NOISE_BASE_FREQ;
  noise.bamp = DEFAULT_NOISE_BASE_AMP;

  double depth = 0.0;

  generate_noise(g_curr_width, g_curr_height, depth, &noise, noise_pixels);

  /// Shaders
  /// -------
  Shader d_shader = create_shader("shaders/basic.vert", "shaders/basic.frag");
  Shader s_shader = create_shader("shaders/screen.vert", "shaders/screen.frag");

  glUseProgram(s_shader.program);
  shader_uniform1i(&s_shader, "screenTexture", 0);
  glUseProgram(0);

  mat4 projection;

  int frame_count = 0;

  iperlin_control_setup(g_curr_width, g_curr_height);

  /// Core loop
  /// ---------
  while (!glfwWindowShouldClose(window)) {
    glm_ortho(0.0f, (float)g_curr_width, (float)g_curr_height, 0.0f, -1.0f, 1.0f, projection);


    glfwPollEvents();
    dgui_update();

    // first pass
    // ---------
    framebuffer_enable(&fbuffer);

    glClearColor(0.13f, 0.14f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    iperlin_control_render(projection);

    /// It is possible to setup a key press check in the coer loop as well

    // second pass
    // -----------
    framebuffer_disable(&fbuffer);
    glViewport(0, 0, g_curr_width, g_curr_height);    // Set viewport to FBO dimensions

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_shader.program);

    glBindVertexArray(screen_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbuffer.texture_color_buffer);
    shader_uniform1i(&s_shader, "screenTexture", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);

    dgui_render();
    glfwSwapBuffers(window);

    frame_count++;
  }

  iperlin_control_shutdown();

  glDeleteVertexArrays(1, &screen_vao);
  glDeleteBuffers(1, &screen_vbo);

  framebuffer_release(&fbuffer);

  dgui_terminate(&dgui);

  release_shader(&d_shader);
  release_shader(&s_shader);


  /// Cleanup
  /// -------
  glfwTerminate();

  return EXIT_SUCCESS;
}
