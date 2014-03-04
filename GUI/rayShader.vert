#version 330

layout(location = 0)in vec2 vertexIn;

const vec2 madd=vec2(0.5,0.5);

out vec2 textureCoord;

void main() 
{
   textureCoord = vertexIn.xy*madd+madd; // scale vertex attribute to [0-1] range
   gl_Position = vec4(vertexIn.xy,0.0,1.0);
}