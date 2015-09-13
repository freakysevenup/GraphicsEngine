#version 120

uniform mat4 camera;
uniform vec3 light_position;
uniform sampler2D m;
uniform float lightPower;

varying vec3 normal0, position0, lightDir0;
varying vec2 texCoords0;

void main (void)
{
	vec4 finalColor;

	vec3 dist = position0 - light_position;
	float brightness = dot(dist, light_position);

	float attenuation = 1.0 / (brightness);

	finalColor = texture2D(m, texCoords0) * attenuation;

	gl_FragColor = finalColor * ( lightPower / attenuation );
}