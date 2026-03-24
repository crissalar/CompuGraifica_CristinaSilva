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
uniform Light light2;  // segunda fuente de luz

uniform sampler2D texture_diffusse;

vec3 calcLight(Light l, vec3 norm, vec3 viewDir)
{
    // Ambient
    vec3 ambient = l.ambient * material.ambient;

    // Diffuse
    vec3 lightDir = normalize(l.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = l.diffuse * diff * material.diffuse;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = l.specular * (spec * material.specular);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcLight(light,  norm, viewDir);
    result     += calcLight(light2, norm, viewDir);  // suma la segunda luz

    color = vec4(result, 1.0f) * texture(texture_diffusse, TexCoords);
}
