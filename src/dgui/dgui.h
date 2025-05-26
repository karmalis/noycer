#ifndef DGUI_H_
#define DGUI_H_

#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

typedef struct dgui dgui;

struct dgui {
ImGuiContext* ctx;
ImGuiIO* io;
};

bool dgui_init(GLFWwindow* window, dgui* gui);
void dgui_render();
void dgui_update();
void dgui_terminate(dgui* gui);

#endif // DGUI_H_
