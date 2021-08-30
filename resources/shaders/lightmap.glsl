#shader vertex
#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 plane;

out vec2 TexCoords;
out vec3 normal;
out vec3 FragPos;
out vec3 position;

void main()
{
    const vec4 vertexPos = model * vec4(aPos,1.0);
	gl_Position = projection * view * vertexPos;
    FragPos = vec3(vertexPos);
    normal = normalize(mat3(transpose(inverse(model))) * aNormal );
    TexCoords = aTexCoords;
	position = vec3(gl_Position);
}

#shader fragment
#version 430 core

uniform int isNightMode;

in vec3 normal, FragPos;
in vec2 TexCoords;
in vec3 position;

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


struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation variables
    float constant;
    float linear;
    float quadratic;

};


uniform Material material;
uniform PointLight light;
uniform vec3 viewPos;
uniform int isReflection;
uniform sampler2D depthBuffer;
uniform sampler2D prevBuffer;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    const vec4 col_diffuse_1 = texture(material.diffuse1, TexCoords);
    const vec4 col_diffuse_2 = texture(material.diffuse2, TexCoords);
    const vec4 col_diffuse_3 = texture(material.diffuse3, TexCoords);
    const vec4 col_diffuse_4 = texture(material.diffuse4, TexCoords);
    const vec4 temp1=mix(col_diffuse_1, col_diffuse_2, 0.5);
    const vec4 temp2=mix(col_diffuse_3, col_diffuse_4, 0.5);

    const vec4 material_diffuse = mix(temp1, temp2, 0.5);
    vec3 ambient = light.ambient * material_diffuse.rgb;

    const vec3 lightDir = normalize(light.position - fragPos);
    const float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material_diffuse.rgb;

    const vec4 col_specular_1 = texture(material.specular1, TexCoords);
    const vec4 col_specular_2 = texture(material.specular2, TexCoords);
    const vec4 material_specular = col_specular_1 * col_specular_2;

    const vec3 reflectDir = reflect(-lightDir, normal);
    const float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material_specular.rgb;


    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			    light.quadratic * (distance * distance));    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


//lights for open gl
#define NR_POINT_LIGHTS 20 
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main()
{    
	const float width = 800.0f;
	vec4 color = texture(depthBuffer,gl_FragCoord.xy/width);
    float prev_depth = color.x;
    float new_depth = pow(gl_FragCoord.z, 40);

	vec3 result = vec3(0.0f);
    const vec4 col_diffuse_1 = texture(material.diffuse1, TexCoords);
    const vec4 col_diffuse_2 = texture(material.diffuse2, TexCoords);
    const vec4 col_diffuse_3 = texture(material.diffuse3, TexCoords);
    const vec4 col_diffuse_4 = texture(material.diffuse4, TexCoords);
    const vec4 temp1=mix(col_diffuse_1, col_diffuse_2, 0.5);
    const vec4 temp2=mix(col_diffuse_3, col_diffuse_4, 0.5);

    const vec4 material_diffuse = mix(temp1, temp2, 0.5);
    const vec3 ambient = light.ambient * material_diffuse.rgb;

    const vec3 lightDir = normalize(light.position - FragPos);
    const float diff = max(dot(normal, lightDir), 0.0);
    const vec3 diffuse = diff * light.diffuse * material_diffuse.rgb;

    const vec4 col_specular_1 = texture(material.specular1, TexCoords);
    const vec4 col_specular_2 = texture(material.specular2, TexCoords);
    const vec4 material_specular = col_specular_1 * col_specular_2;

    const vec3 viewDir = normalize(viewPos - FragPos);
    const vec3 reflectDir = reflect(-lightDir, normal);
    const float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    const vec3 specular = spec * light.specular * material_specular.rgb;


	if(isNightMode == 0){
    	result = (ambient + diffuse + specular) * 1.8;
	}
	else{	
		for(int i = 0; i < NR_POINT_LIGHTS; i++){
	       result += CalcPointLight(pointLights[i], normal, FragPos, viewDir); 
		}
	}
	float alphaValue;
	if(isReflection == 0){
		alphaValue = col_diffuse_2.w;
	}
	else{
		alphaValue = 0.5f;
		result *= vec3(0.6); 
	}
    if (new_depth <= prev_depth + 0.0032f){
  	  FragColor = vec4(result, alphaValue);
	}
	else{
		FragColor = texture(prevBuffer,gl_FragCoord.xy/width);
	}
} 
