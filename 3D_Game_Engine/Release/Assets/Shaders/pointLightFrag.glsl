#version 150

uniform mat4 model;
uniform vec3 lightSource;
uniform sampler2D mySampler;

uniform float lightIntensity;
//uniform vec4 lightColour;

in vec3 vertex_light_position;
in vec3 vertex_normal;
in vec2 textureCoords0;

out vec4 finalColour;

void main() 
{

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMatrix * vertex_normal);

	vec3 fragPosition = (model * vec4(vertex_light_position, 1.0)).xyz;

	vec3 lightToSurface = lightSource - fragPosition; // may have to switch this around

	float distance = length(lightToSurface);
	float attenuation = 1.0 / distance;


	float brightness = dot(normal, lightToSurface) / (length(lightToSurface) * length(normal));
	brightness = clamp(brightness, 0, 1);

	vec4 surfaceColour = texture(mySampler, textureCoords0);

	//finalColour = (attenuation * vec4(lightSource, 1.0) * max(0.0, dot(normal, lightToSurface)));

	finalColour = lightIntensity * vec4((brightness * surfaceColour.rgb), surfaceColour.a);


}