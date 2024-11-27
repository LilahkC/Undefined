#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int PickingFragColor;

struct DirLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool isUsed;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
    
    bool isUsed;
};

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

// LIGHT
#define NBR_OF_DIR_LIGHT 1
#define NBR_OF_POINT_LIGHT 16
#define NBR_OF_SPOT_LIGHT 16

uniform vec3 viewPos;
uniform DirLight dirLights[NBR_OF_DIR_LIGHT];
uniform PointLight pointLights[NBR_OF_POINT_LIGHT];
uniform SpotLight spotLights[NBR_OF_SPOT_LIGHT];

uniform int EntityID;

// TEXTURE
uniform sampler2D texture0;

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture0, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture0, TexCoord));

    return  (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture0, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture0, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture0, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture0, TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 result;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // dirlight
    for (int i = 0; i < NBR_OF_DIR_LIGHT; i++)
    {
         result += CalcDirLight(dirLights[i], norm, viewDir);
    }
    
    // point light
    for (int i = 0; i < NBR_OF_POINT_LIGHT; i++)
    {
        if (pointLights[i].isUsed)
        {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    // point light
    for (int i = 0; i < NBR_OF_SPOT_LIGHT; i++)
    {
        if (spotLights[i].isUsed)
        {
            result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
        }
    }
    
    FragColor = vec4(result, 1.0);
    PickingFragColor = EntityID;
}