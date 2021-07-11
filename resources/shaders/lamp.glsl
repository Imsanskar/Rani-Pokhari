#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;   // the position variable has attribute position 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 lightColour;
void main()
{
    FragColor = vec4(lightColour,  1.0f);
}