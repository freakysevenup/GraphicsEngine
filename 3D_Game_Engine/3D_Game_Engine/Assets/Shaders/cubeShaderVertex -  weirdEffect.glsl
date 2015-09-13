#version 150

in vec3 position;
in vec3 normal;

uniform mat4 model;
uniform vec3 lightSource;

out vec3 vertex_light_position;
out vec3 vertex_normal;
out vec3 position0;


uniform mat4 camera;

void main(void)
{
	position0 = position;

    	vertex_normal = normalize((model * vec4(normal, 1.0)).xyz);
    	vertex_light_position = normalize(lightSource - vec4(position, 1.0).xyz);

    	gl_Position = camera * vec4(position, 1.0);
}