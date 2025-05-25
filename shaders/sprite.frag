#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D spriteTexture;
uniform vec3 spriteColor;

void main()
{
    FragColor = vec4(spriteColor, 1.0) * texture(spriteTexture, TexCoords);
    if (FragColor.a < 0.1) discard;
    //FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
