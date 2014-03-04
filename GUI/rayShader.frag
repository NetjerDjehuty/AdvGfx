#version 330

in vec2 textureCoord;

uniform sampler2D diffTex;

out vec4 fragColor;

void main() 
{
   vec4 color1 = texture2D(diffTex,textureCoord);
   fragColor = color1;
}