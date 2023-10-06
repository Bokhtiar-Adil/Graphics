#version 330 core


struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient; // K_amb
    vec3 diffuse; // K_diff
    vec3 specular; // K_spec
    float shininess; // n
};


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 

uniform Light light;
uniform Material material;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient; // I_amb = K_amb * I_src
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f); // only positive values of normalized (normal * light_direction)
    vec3 diffuse = light.diffuse * (diff * material.diffuse); // I_diff = K_diff * (normal * light_direction) * I_src    

    // specular
    float specularStr = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // negate it as lightDir is obj to src -> need src to obj
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * material.specular); // I_spec = k_spec * (reflection_direction * normal)^shininess * I_src

    FragColor = vec4((ambient + diffuse + specular), 1.0f); // I_obj = I_amb + I_diff + I_spec

}