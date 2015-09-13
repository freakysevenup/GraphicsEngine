#version 330
 
// Inputs to frag shader.
in vec3 vs_world;
in vec3 vs_normal;
in vec2 vs_texCoords0;
 
// Outputs.
out vec4 out_color;
 
// Light information.
uniform vec3  lightPosition;
uniform vec3  lightDirection;
uniform float lightRange;
uniform float lightCosInnerAngle;
uniform float lightCosOuterAngle;
uniform vec3  lightDiffuseColor;
uniform float lightDiffuseIntensity;
uniform vec3  lightSpecularColor;
uniform float lightSpecularIntensity;

// Texture info
uniform sampler2D tex;
 
// Camera information.
//uniform vec3 cameraPosition;
 
// Material information.
uniform vec3  matDiffuseColor;
uniform float matSpecularPower;
 
void main( ) {
  // Light vector.
  vec3 L = lightPosition - vs_world;
  // Length of light vector (used for height attenuation).;
  float distToLight = length(L);
  // Normalize light vector.
  L = normalize(L);
 
  // Compute smoothed dual-cone effect.
  float cosDir = dot(L, -lightDirection);
  float spotEffect = smoothstep(lightCosOuterAngle, lightCosInnerAngle, cosDir);
 
  // Compute height attenuation based on distance from earlier.
  float heightAttenuation = smoothstep(lightRange, 1.0f, distToLight);
 
  // Normal.
  vec3 N = normalize(vs_normal);
 
  // Diffuse lighting.
  float diffuseLight = max(dot(N, L), 0.0f);
  vec3 diffuse = (diffuseLight * lightDiffuseColor) * lightDiffuseIntensity;
 
  // Specular lighting.
  float specularLight = 0.0f;
  if( matSpecularPower > 0.0f ) {
    vec3 V = normalize(vs_world);//cameraPosition - 
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, N);
    specularLight = pow(clamp(dot(R, H), 0.0f, 1.0f), matSpecularPower);
  }
  vec3 specular = (specularLight * lightSpecularColor) * lightSpecularIntensity;
 
  // Final combined color.
  vec3 finalColor = ((diffuse + specular) * matDiffuseColor) * spotEffect * heightAttenuation;
 
  out_color = vec4(finalColor, 1.0f); //  texture2D(tex, vs_texCoords0) + 
}