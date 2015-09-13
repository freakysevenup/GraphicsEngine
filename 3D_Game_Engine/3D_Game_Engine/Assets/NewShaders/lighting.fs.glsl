#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct BaseLight
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	Attenuation atten;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutoff;
};

uniform int gNumPointLights;                                                                
uniform int gNumSpotLights; 
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D gSampler;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
	vec4 AmbientColor = vec4(Light.colour * Light.ambientIntensity, 1.0f);
	float DiffuseFactor = dot(Normal, -LightDirection);

	vec4 DiffuseColor  = vec4(0, 0, 0, 0);
	vec4 SpecularColor = vec4(0, 0, 0, 0);

	if (DiffuseFactor > 0) 
	{
		DiffuseColor = vec4(Light.colour * Light.diffuseIntensity * DiffuseFactor, 1.0f);

		vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));
		float SpecularFactor = dot(VertexToEye, LightReflect);
		if (SpecularFactor > 0) 
		{
			SpecularFactor = pow(SpecularFactor, gSpecularPower);
			SpecularColor = vec4(Light.colour * gMatSpecularIntensity * SpecularFactor, 1.0f);
		}
	}

	return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                           
    return CalcLightInternal(gDirectionalLight.base, gDirectionalLight.direction, Normal); 
}                                                                                           


vec4 CalcPointLight(PointLight one, vec3 Normal)
{
	vec3 LightDirection = WorldPos0 - one.position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);

	vec4 Color = CalcLightInternal(one.base, LightDirection, Normal);
	float Attenuation =  one.atten.constant +
	one.atten.linear * Distance +
	one.atten.exp * Distance * Distance;

	return Color / Attenuation;
}

vec4 CalcSpotLights(SpotLight one, vec3 Normal)
{
	vec3 LightToPixel = normalize(WorldPos0 - one.base.position);
	float SpotFactor = dot(LightToPixel, one.direction);

	if (SpotFactor > one.cutoff)
	{
		vec4 colour = CalcPointLight(one.base, Normal);
		return colour * (1.0, (1.0 - SpotFactor) * 1.0/(1.0 - one.cutoff));
	}
	else
	{
		return vec4(0.0, 0.0, 0.0, 0.0);
	}
}

void main()
{
	vec3 Normal = normalize(Normal0);
	vec4 TotalLight = CalcDirectionalLight(Normal);

	for (int i = 0 ; i < gNumPointLights ; i++) 
	{
		TotalLight += CalcPointLight(gPointLights[i], Normal);
	}

	for (int i = 0; i < gNumSpotLights; i++)
	{
		TotalLight += CalcSpotLights(gSpotLights[i], Normal);
	}

	FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;
}
