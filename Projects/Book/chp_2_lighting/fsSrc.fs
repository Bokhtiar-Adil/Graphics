#version 330 core


struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight  {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    vec3 ambient; // K_amb
    vec3 diffuse; // K_diff
    vec3 specular; // K_spec
    float shininess; // n
};

struct Materialtex {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords; 

// uniform PointLight light;
// uniform DirectionalLight light;
uniform SpotLight light;

uniform Material material;
uniform Materialtex materialtex;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // vec3 lightDirDir = normalize(-light.direction);
    // ambient
    vec3 ambient = light.ambient * material.ambient; // I_amb = K_amb * I_src
    vec3 ambienttex = light.ambient * texture(materialtex.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f); // only positive values of normalized (normal * light_direction)
    // float diff = max(dot(norm, lightDirDir), 0.0f); // only positive values of normalized (normal * light_direction)
    vec3 diffuse = light.diffuse * (diff * material.diffuse); // I_diff = K_diff * (normal * light_direction) * I_src    
    vec3 diffusetex = light.diffuse * diff * texture(materialtex.diffuse, TexCoords).rgb; // I_diff = K_diff * (normal * light_direction) * I_src    

    // specular
    float specularStr = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // negate it as lightDir is obj to src -> need src to obj
    // vec3 reflectDir = reflect(-lightDirDir, norm); // negate it as lightDir is obj to src -> need src to obj
    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * (spec * material.specular); // I_spec = k_spec * (reflection_direction * normal)^shininess * I_src
    float spectex = pow(max(dot(reflectDir, viewDir), 0.0f), materialtex.shininess);
    vec3 speculartex = light.specular * spectex * texture(materialtex.specular, TexCoords).rgb; // I_spec = k_spec * (reflection_direction * normal)^shininess * I_src

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);    
    diffusetex = diffusetex * attenuation;
    speculartex = speculartex * attenuation;

    // spot light specialities
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutoff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffusetex  *= intensity;
    speculartex *= intensity;

    // FragColor = vec4((ambient + diffuse + specular), 1.0f); // I_obj = I_amb + I_diff + I_spec
    FragColor = vec4((ambienttex + diffusetex + speculartex), 1.0f); // I_obj = I_amb + I_diff + I_spec

}