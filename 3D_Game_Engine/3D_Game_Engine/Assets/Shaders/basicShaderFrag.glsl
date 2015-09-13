#version 120

varying vec2 textureCoords0;
varying vec3 normal0;

uniform sampler2D mySampler;

void main(void)
{
	gl_FragColor = texture2D(mySampler, textureCoords0);// + clamp(dot(-vec3(0.0, 0.0, 0.0), normal0), 0.0, 1.0);
}