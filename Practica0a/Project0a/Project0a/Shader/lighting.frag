#version 330 core
struct Material
{
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light light2;
uniform sampler2D texture_diffuse;

void main()
{
    // Primera luz
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position - FragPos);

// Ambient
vec3 ambient  = light.ambient * material.diffuse;

// Diffuse
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse  = light.diffuse * diff * material.diffuse;

// Specular
vec3 viewDir    = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * (spec * material.specular);

// Segunda luz
vec3 light2Dir = normalize(light2.position - FragPos);

// Ambient (light2)
vec3 ambient2  = light2.ambient * material.diffuse;

// Diffuse (light2)
float diff2 = max(dot(norm, light2Dir), 0.0);
vec3 diffuse2  = light2.diffuse * diff2 * material.diffuse;

// Specular (light2)
vec3 reflectDir2 = reflect(-light2Dir, norm);
float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess);
vec3 specular2 = light2.specular * (spec2 * material.specular);

// Resultado final sumando ambas luces
vec3 result = ambient + diffuse + specular + ambient2 + diffuse2 + specular2;
FragColor = vec4(result, 1.0);
}