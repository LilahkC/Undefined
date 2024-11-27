#version 450

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, TexCoord);
}