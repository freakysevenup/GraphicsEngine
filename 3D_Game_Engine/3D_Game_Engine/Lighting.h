#pragma once
#include "ShaderManager.h"
#include <GLM/glm.hpp>

struct BaseLight
{
	glm::vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;

	BaseLight()
	{
		colour = glm::vec3(0.0f, 0.0f, 0.0f);
		ambientIntensity = 0.0f;
		diffuseIntensity = 0.0f;
	}
};

struct DirectionalLight :
	public BaseLight
{
	glm::vec3 direction;

	DirectionalLight()
	{
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight :
	public BaseLight
{
	glm::vec3 position;

	struct
	{
		float constant;
		float linear;
		float exp;
	} attenuation;

	PointLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		attenuation.constant = 0.0f;
		attenuation.linear = 0.0f;
		attenuation.exp = 0.0f;
	}
};

struct SpotLight :
	public PointLight
{
	glm::vec3 direction;
	float cutoff;

	SpotLight()
	{
		direction = glm::vec3(0.0f);
		cutoff = 0.0f;
	}
};



class Lighting :
	public ShaderManager
{

public:

	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	Lighting();

	virtual bool init() override;

	void setWorldViewProjection(const glm::mat4 & worldViewProjection);
	void setWorldMatrix(const glm::mat4 & worldViewProjection);
	void setTextureID(unsigned int textureID);
	void setDirectionalLight(const DirectionalLight & light);
	void SetPointLights(unsigned int NumLights, const PointLight * pLights);
	void SetSpotLights(unsigned int NumLights, const SpotLight * pLights);
	void setEyeWorldPos(const glm::vec3 & eyeWorldPos);
	void setMatSpecularIntensity(float intensity);
	void setMatSpecularPower(float power);

private:

	GLuint m_worldViewProjectionLocation;
	GLuint m_samplerLocation;
	GLuint m_worldMatrixLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	struct
	{
		GLuint colour;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} m_dirLightLocation;

	struct
	{
		GLuint colour;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;

		struct
		{
			GLuint constant;
			GLuint linear;
			GLuint exp;
		} Atten;

	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct 
	{
		GLuint colour;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint direction;
		GLuint cutoff;

		struct 
		{
			GLuint constant;
			GLuint linear;
			GLuint exp;
		} Atten;

	} m_spotLightsLocation[MAX_SPOT_LIGHTS];

};