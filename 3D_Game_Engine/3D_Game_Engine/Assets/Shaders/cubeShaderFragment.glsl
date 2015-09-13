#version 150

in vec4 position0;
in vec2 textureCoords0;

out vec4 finalColour;

uniform samplerCube mySampler;

void main(void)
{
	finalColour = texture(mySampler, (vec3(textureCoords0, 0.0) + position0.xyz));
}
