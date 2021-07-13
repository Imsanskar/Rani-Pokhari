#shader vertex
#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 normal;
out vec3 FragPos;

void main()
{
    const vec4 vertexPos = model * vec4(aPos,1.0);
     gl_Position = projection * view * model * trans * vec4(aPos, 1.0);
    FragPos = vec3(vertexPos);
    normal = normalize(mat3(transpose(inverse(model))) * aNormal );
    TexCoords = aTexCoords;
}

#shader fragment
#version 430 core
in vec3 normal, FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D diffuse4;
    sampler2D specular1;
    sampler2D specular2;
    
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation variables
    //float constant;
    //float linear;
    //float quadratic;

};


uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    const vec4 col_diffuse_1 = texture(material.diffuse1, TexCoords);
    const vec4 col_diffuse_2 = texture(material.diffuse2, TexCoords);
    const vec4 col_diffuse_3 = texture(material.diffuse3, TexCoords);
    const vec4 col_diffuse_4 = texture(material.diffuse4, TexCoords);
    const vec4 temp1=mix(col_diffuse_1, col_diffuse_2, 0.5);
    const vec4 temp2=mix(col_diffuse_3, col_diffuse_4, 0.5);

    const vec4 material_diffuse = mix(temp1, temp2, 0.5);
    const vec3 ambient = light.ambient * material_diffuse.rgb;

    const vec3 lightDir = normalize(light.position );
    const float diff = max(dot(normal, lightDir), 0.0);
    const vec3 diffuse = diff * light.diffuse * material_diffuse.rgb;

    const vec4 col_specular_1 = texture(material.specular1, TexCoords);
    const vec4 col_specular_2 = texture(material.specular2, TexCoords);
    const vec4 material_specular = col_specular_1 * col_specular_2;

    const vec3 viewDir = normalize(viewPos - FragPos);
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    const vec3 specular = spec * light.specular * material_specular.rgb;

    // for light attenuation
    //const float distance = length(light.position - FragPos);
   // const float attenuation = 1.0 / (light.constant + light.linear * distance +
     //   light.quadratic * (distance * distance));

    //const vec3 result = ((ambient * attenuation) + (diffuse * attenuation) + (specular * attenuation));
     vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, col_diffuse_2.w);

} 
