#version 330

// Inputs from C++.
in vec3 position;
in vec3 normal;
in vec2 texCoords;

// Outputs to frag shader.
out vec3 vs_world;
out vec3 vs_normal;
out vec2 vs_texCoords0;

// Transforms from C++.
uniform mat4 camera;
//uniform mat4 model;

void main( ) {
  gl_Position = (camera * vec4(position, 1.0f));
  vs_world    = (camera * vec4(position, 1.0f)).xyz;
  vs_normal   = (camera * vec4(normal,   0.0f)).xyz;
  vs_texCoords0 = texCoords;
}
