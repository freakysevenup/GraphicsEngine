#version 120


attribute vec3 position;

attribute vec3 normal;


varying vec3 outNormal;

varying vec3 worldVertexPosition;

varying vec3 worldNormalDirection;


uniform mat4 model;

uniform mat4 camera;


void main()

{
	
	gl_Position = camera * vec4(position, 1.0);
	
	worldVertexPosition = vec3(model * vec4(position, 1.0));
	
	worldNormalDirection = mat3(model) * normal;

}
