#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 normal;
out vec3 FragPos;

uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * trans * vec4(aPos, 1.0);
    //TexCoord = aTexCoord;
    normal = aNormal;
    //normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}


#shader fragment
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform Material material;


in vec3 FragPos;

//specular lighting
uniform vec3 viewPos;

void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * material.ambient;

    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);



    vec3 result = (ambient + diffuse + specular);
    //FragColor = texture(texture1, TexCoord);
    FragColor = vec4(result, 1.0f);;
}