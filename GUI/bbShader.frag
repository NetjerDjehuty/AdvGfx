#version 330

in vec2 texCoord;

uniform sampler2D diffTex;
uniform vec3 photonColor;
uniform float pow;

out vec4 fragColor;

void main() 
{
   float c = texture2D(diffTex,texCoord);
   fragColor = vec4(photonColor*pow*c, c);
}