#version 150

in vec3 position;
in vec3 normal;
in vec2 textureCoords;

out vec4 position0;
out vec2 textureCoords0;
out vec3 normal0;

uniform mat4 camera;

void main(void)
{

	normal0 = normal;
	textureCoords0 = textureCoords;

	position0 = vec4(position, 0.0);

    gl_Position = camera * vec4(position, 1.0);
}