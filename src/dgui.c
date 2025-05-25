#include "dgui.h"

bool dgui_init(GLFWwindow* window, dgui* gui) {
    gui->ctx = igCreateContext(NULL);
    gui->io = igGetIO_ContextPtr(gui->ctx);

    const char* glsl_version = "#version 460 core";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);

    return true;
}

void dgui_render() {
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void dgui_update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    igBegin("Test", NULL, 0);
    igText("Test");
    igButton("TestB", (struct ImVec2){0,0});
    igEnd();

    igShowDemoWindow(NULL);
}

void dgui_terminate(dgui* gui) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(gui->ctx);
}
