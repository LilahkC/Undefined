#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	FragPos = aPos;
	Normal = aNormal;
	TexCoord = aTexCoord;

	gl_Position = vec4(FragPos, 1.0);
}

