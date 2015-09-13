#version 150

in vec3 vertex_light_position;
in vec3 position0;

out vec4 finalColour;

uniform mat4 model;
uniform samplerCube mySampler;

void main(void)
{
	vec3 fragPosition = (model * vec4(vertex_light_position, 0.0)).xyz;

	finalColour = texture(mySampler, -fragPosition + position0);
}
