#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 texc;
layout(location = 2)in vec3 norm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(vert,1);
}