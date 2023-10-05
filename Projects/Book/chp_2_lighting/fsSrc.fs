#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f); // only positive values of normalized (normal * light_direction)
    vec3 diffuse = diff * lightColor; // I_diff = K_diff * (normal * light_direction) * I_src
    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColor; // I_amb = K_amb * I_src
    FragColor = vec4((ambient + diffuse) * objectColor, 1.0f); // I_obj = I_amb + I_diff

}