#version 330

uniform sampler2D diffTex;

in vec3 worldPosition;
in vec2 texCoord;
in vec3 normal;

out vec4 fragColor;

vec3 lightPos = vec3(50.0, 50.0, 50.0);
vec3 ambient = vec3(0.1);

void main()
{
	vec3 n = normalize(normal);
	vec3 l = normalize(lightPos - worldPosition);

	//vec3 diff = texture2D( diffTex, texCoord ).xyz * max(dot(n,l), 0.0);
	vec3 diff = vec3(1) * max(dot(n,l), 0.0);
	diff += ambient;
	diff = clamp(diff, 0.0, 1.0);

    fragColor = vec4(diff, 1.0);
}