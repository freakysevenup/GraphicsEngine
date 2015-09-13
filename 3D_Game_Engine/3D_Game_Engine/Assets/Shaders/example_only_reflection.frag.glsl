#version 120


varying vec3 worldVertexPosition;

varying vec3 worldNormalDirection;

uniform samplerCube cubeMap;

uniform vec3 cameraPosition;



void main()

{
	
	vec3 incident = normalize(worldVertexPosition - cameraPosition);
	
	vec3 normal = normalize(worldNormalDirection);
	
	vec3 reflected = normalize(refract(incident, normal, 1.1));
	
	gl_FragColor = textureCube(cubeMap, reflected);

}
