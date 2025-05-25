#version 460 core
in vec3 v_debug_pos; // This will receive the interpolated a_pos from the vertex shader
out vec4 FragColor;

void main() {
    // Map NDC (-1 to 1) to a color (0 to 1)
    // If a_pos is truly from -0.5 to 0.5, you'll see a small rainbow in the center.
    // If it's 0 to 1, you'll see a rainbow in the top-right.
    float r = (v_debug_pos.x + 1.0) / 2.0; // Map x from -1 to 1 to 0 to 1
    float g = (v_debug_pos.y + 1.0) / 2.0; // Map y from -1 to 1 to 0 to 1
    FragColor = vec4(r, g, 0.0, 1.0); // R (x), G (y)
}
