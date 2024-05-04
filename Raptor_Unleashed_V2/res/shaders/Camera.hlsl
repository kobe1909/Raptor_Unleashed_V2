#shader vertex
#version 330 core

layout(location = 0) in
vec3 position;
layout(location = 1) in
vec3 inNormal;
layout(location = 2) in
vec2 inTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out
vec3 normal;
out
vec3 FragPos;
out 
vec2 TexCoords;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0));
    normal = mat3(transpose(inverse(model))) * inNormal;
    TexCoords = inTexCoords;
    gl_Position = proj * view * vec4(FragPos, 1.0);
};


#shader fragment
#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    float shininess;
};

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
    float Linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out
vec4 color;

uniform vec3 cameraPos;

uniform Material material;

uniform DirLight dirLight;
#define NR_POINTS_LIGHTS 1
uniform PointLight pointLights[NR_POINTS_LIGHTS];
uniform SpotLight spotLight;

in
vec3 normal;
in
vec3 FragPos;
in
vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// Properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    
    // Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Point lights
    for (int i = 0; i < NR_POINTS_LIGHTS; i++) 
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    
    // Spot lights
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    color = vec4(result, 1.0);
};


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.Linear * distance + light.quadratic * distance * distance);
    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight spotLight, vec3 norm, vec3 FragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    
    if (theta > spotLight.innerCutOff)
    {
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
    
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        vec3 ambient = spotLight.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
        vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords)) * intensity;
        vec3 specular = spotLight.specular * spec * vec3(texture(material.texture_specular1, TexCoords)) * intensity;
        
        return (ambient + diffuse + specular);
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
}