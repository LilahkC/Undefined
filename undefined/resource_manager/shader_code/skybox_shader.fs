#version 450 core
out vec4 FragColor;
out int EntityID;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    EntityID = -1;
}