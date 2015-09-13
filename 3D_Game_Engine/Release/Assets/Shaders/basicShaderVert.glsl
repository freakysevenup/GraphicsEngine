#version 120

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;

varying vec2 textureCoords0;
varying vec3 normal0;

uniform mat4 camera;

void main(void)
{
	gl_Position = (camera * vec4(position, 1.0));
	textureCoords0 = textureCoords;
	normal0 = (camera * vec4(normal.xyz, 0.0)).xyz;
}