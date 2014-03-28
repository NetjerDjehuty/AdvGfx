#version 330

layout(location = 0)in vec2 squareVertices;

uniform vec3 particleCenter_worldspace;

const vec2 BillboardSize = vec2(0.5);

uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;

void main()
{
	vec3 CameraRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 CameraUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);

	vec3 vertexPos_world = 
			particleCenter_worldspace
			+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
			+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;



	gl_Position = proj * view * vec4(vertexPos_world, 1);


	texCoord = squareVertices + vec2(0.5);
}