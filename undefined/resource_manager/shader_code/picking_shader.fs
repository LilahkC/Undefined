#version 450 core

layout (location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}