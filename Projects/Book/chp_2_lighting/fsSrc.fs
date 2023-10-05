#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    
    float ambientStr = 0.1f;
    vec3 ambient = ambientStr * lightColor; // I_amb = K_amb * I_src
    
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f); // only positive values of normalized (normal * light_direction)
    vec3 diffuse = diff * lightColor; // I_diff = (normal * light_direction) * I_src    

    float specularStr = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // negate it as lightDir is obj to src -> need src to obj
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32.0f);
    vec3 specular = specularStr * spec * lightColor; // (reflection_direction * normal)^n * I_src

    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f); // I_obj = K_amb * I_amb + K_diff * I_diff + K_spec * I_spec

}