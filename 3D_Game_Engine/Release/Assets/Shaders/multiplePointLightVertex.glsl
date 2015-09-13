#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoords;

uniform mat4 camera;
uniform vec3 light_position;

varying vec3 normal0, position0, lightDir0;
varying vec2 texCoords0;

void main()
{	
	position0 = position;
	texCoords0 = texCoords;
	normal0 = normal;

	lightDir0 = light_position - position;

	gl_Position = camera * vec4(position, 1.0);
}