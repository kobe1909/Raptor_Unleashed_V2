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
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out
vec4 color;

uniform vec3 cameraPos;

uniform Material material;
uniform Light light;

in
vec3 normal;
in
vec3 FragPos;
in
vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	// ambient
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	// diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));

	// specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	
	// combient
    vec3 result = ambient + diffuse + specular;
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