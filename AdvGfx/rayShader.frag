#version 330

varying vec2 textureCoord;
uniform sampler2D diffTex;

void main() 
{
   vec4 color1 = texture2D(diffTex,textureCoord);
   gl_FragColor = color1;
}