#version 150

in vec4 position0;
in vec2 textureCoords0;
in vec3 normal0;

out vec4 finalColour;

uniform vec3 viewDirection;
uniform samplerCube mySampler;
uniform samplerCube boxTex;

const float mother_pearl_brightness = 1.5;

void main(void)
{


vec3  fvNormal         = normalize(normal0);
vec3  fvViewDirection  = normalize(viewDirection);
vec3  reflection	   = reflect(fvViewDirection, fvNormal);
vec3  fvReflection     = normalize(reflection);

float lerp = dot(normal0, viewDirection) - dot(reflection, viewDirection) / dot(normal0, viewDirection) + dot(reflection, viewDirection);

if (lerp < 0.0)
{
	lerp*= -1;
}

float view_dot_normal = max(dot(fvNormal, fvViewDirection), 1.0 - lerp);
float view_dot_normal_inverse = 1.0 - view_dot_normal;

finalColour = texture(boxTex, (vec3(textureCoords0, 0.0) + position0.xyz));// * view_dot_normal;

finalColour.r += mother_pearl_brightness * texture(boxTex, fvReflection + vec3(1.0 - lerp, 0.0, 0.0) * view_dot_normal_inverse).r * (1.0 - view_dot_normal);
finalColour.g += mother_pearl_brightness * texture(boxTex, fvReflection + vec3(0.0, 1.0 - lerp, 0.0) * view_dot_normal_inverse).g * (1.0 - view_dot_normal);
finalColour.b += mother_pearl_brightness * texture(boxTex, fvReflection + vec3(0.0, 0.0, 1.0 - lerp) * view_dot_normal_inverse).b * (1.0 - view_dot_normal);

}
