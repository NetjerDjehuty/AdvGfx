#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 texc;
layout(location = 2)in vec3 norm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 worldPosition;
out vec2 texCoord;
out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(vert,1);
	
	worldPosition = vec3(view * model * vec4(vert,1));
	texCoord = texc;
	normal = vec3(view * model * vec4(norm, 0));

}