#shader vertex
#version 330 core

layout(location = 0) in
vec3 position;
layout(location = 1) in
vec3 inNormal;
layout(location = 2) in
vec2 inTexCoords;
layout(location = 3) in
vec3 inTangents;
layout(location = 4) in
vec3 inBitangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 texCoords;
    mat3 TBN;
} vs_out;

void main()
{
    vs_out.fragPos = vec3(model * vec4(position, 1.0));
    vs_out.normal = mat3(transpose(inverse(model))) * inNormal;
    vs_out.texCoords = inTexCoords;
    gl_Position = proj * view * vec4(vs_out.fragPos, 1.0);
    
    vec3 T = normalize(vec3(model * vec4(inTangents,   0.0) ) );
    vec3 B = normalize(vec3(model * vec4(inBitangents, 0.0) ) );
    vec3 N = normalize(vec3(model * vec4(inNormal,     0.0) ) );
    
    vs_out.TBN = mat3(T, B, N);
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

layout(location = 0) out vec4 color;

uniform vec3 cameraPos;

uniform Material material;

uniform DirLight dirLight;
#define NR_POINTS_LIGHTS 1
uniform PointLight pointLights[NR_POINTS_LIGHTS];
uniform SpotLight spotLight;

in VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 texCoords;
    mat3 TBN;
} fs_in;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// Properties
    vec3 norm = normalize(fs_in.TBN * (texture(material.texture_normal1, fs_in.texCoords).rgb * 2.0 - 1.0));
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos);
    
    // Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Point lights
    for (int i = 0; i < NR_POINTS_LIGHTS; i++) 
        result += CalcPointLight(pointLights[i], norm, fs_in.fragPos, viewDir);
    
    // Spot lights
    result += CalcSpotLight(spotLight, norm, fs_in.fragPos, viewDir);
    
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
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.texCoords));
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
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.texCoords));
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
        float diff = max(dot(norm, lightDir), 0.0);
    
        // specular shading
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        vec3 ambient = spotLight.ambient * vec3(texture(material.texture_diffuse1, fs_in.texCoords));
        vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.texCoords)) * intensity;
        vec3 specular = spotLight.specular * spec * vec3(texture(material.texture_specular1, fs_in.texCoords)) * intensity;
        
        return (ambient + diffuse + specular);
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
}