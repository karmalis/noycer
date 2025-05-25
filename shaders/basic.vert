#version 460 core
layout (location = 0) in vec3 a_pos;

out vec3 v_debug_pos; // Output a_pos to fragment shader

void main() {
    v_debug_pos = a_pos; // Pass a_pos directly to the fragment shader
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
}
