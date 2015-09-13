#version 150


in vec3 position;
in vec3 normal;
in vec2 textureCoords;

uniform mat4 camera;
uniform mat4 model;
uniform vec3 lightSource;

out vec3 vertex_light_position;
out vec3 vertex_normal;
out vec2 textureCoords0;

void main() 
{            
	textureCoords0 = textureCoords;
	vertex_normal = normal;

    //vertex_normal = normalize((model * vec4(normal, 1.0)).xyz);
    vertex_light_position = normalize(lightSource - vec4(position, 1.0).xyz);

    gl_Position = camera * vec4(position, 1.0);
}