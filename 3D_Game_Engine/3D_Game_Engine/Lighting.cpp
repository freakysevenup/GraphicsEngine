#include <limits>
#include <string>
#include "Lighting.h"


Lighting::Lighting()
{
}

bool Lighting::init()
{
	bool ret = true;

	if (!ShaderManager::init())
	{
		ret = false;
	}

	if (!addShader(GL_VERTEX_SHADER, "./Assets/NewShaders/lighting.vs.glsl"))
	{
		ret = false;
	}

	if (!addShader(GL_FRAGMENT_SHADER, "./Assets/NewShaders/lighting.fs.glsl"))
	{
		ret = false;
	}

	if (!finalize())
	{
		ret = false;
	}

	m_worldViewProjectionLocation = getUniformLocation("gWVP");
	m_worldMatrixLocation = getUniformLocation("gWorld");
	m_samplerLocation = getUniformLocation("gSampler");
	m_dirLightLocation.colour = getUniformLocation("gDirectionalLight.base.colour");
	m_dirLightLocation.ambientIntensity = getUniformLocation("gDirectionalLight.base.ambientIntensity");
	m_dirLightLocation.direction = getUniformLocation("gDirectionalLight.direction");
	m_dirLightLocation.diffuseIntensity = getUniformLocation("gDirectionalLight.base.diffuseIntensity");
	m_eyeWorldPosLocation = getUniformLocation("gEyeWorldPos");
	m_matSpecularIntensityLocation = getUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = getUniformLocation("gSpecularPower");
	m_numPointLightsLocation = getUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = getUniformLocation("gNumSpotLights");

	if (m_dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION
		|| m_worldViewProjectionLocation == INVALID_UNIFORM_LOCATION
		|| m_worldMatrixLocation == INVALID_UNIFORM_LOCATION
		|| m_samplerLocation == INVALID_UNIFORM_LOCATION
		|| m_dirLightLocation.colour == INVALID_UNIFORM_LOCATION
		|| m_dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION
		|| m_dirLightLocation.direction == INVALID_UNIFORM_LOCATION
		|| m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION
		|| m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION
		|| m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION
		|| m_numPointLightsLocation == INVALID_UNIFORM_LOCATION
		|| m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION)
	{
		ret = false;
	}

	for (unsigned int i = 0; i < sizeof(m_pointLightsLocation) / sizeof(m_pointLightsLocation[0]); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].base.colour", i);
		m_pointLightsLocation[i].colour = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].base.ambientIntensity", i);
		m_pointLightsLocation[i].ambientIntensity = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].position", i);
		m_pointLightsLocation[i].position = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].base.diffuseIntensity", i);
		m_pointLightsLocation[i].diffuseIntensity = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].atten.constant", i);
		m_pointLightsLocation[i].Atten.constant = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].atten.linear", i);
		m_pointLightsLocation[i].Atten.linear = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].atten.exp", i);
		m_pointLightsLocation[i].Atten.exp = getUniformLocation(Name);

		if (m_pointLightsLocation[i].colour == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].Atten.constant == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].Atten.linear == INVALID_UNIFORM_LOCATION ||
		m_pointLightsLocation[i].Atten.exp == INVALID_UNIFORM_LOCATION)
		{
			ret = false;
		}
	}

	for (unsigned int i = 0; i < sizeof(m_spotLightsLocation) / sizeof(m_spotLightsLocation[0]); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.base.colour", i);
		m_spotLightsLocation[i].colour = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.base.ambientIntensity", i);
		m_spotLightsLocation[i].ambientIntensity = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.position", i);
		m_spotLightsLocation[i].position = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].direction", i);
		m_spotLightsLocation[i].direction = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].cutoff", i);
		m_spotLightsLocation[i].cutoff = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.base.diffuseIntensity", i);
		m_spotLightsLocation[i].diffuseIntensity = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.atten.constant", i);
		m_spotLightsLocation[i].Atten.constant = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.atten.linear", i);
		m_spotLightsLocation[i].Atten.linear = getUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gSpotLights[%d].base.atten.exp", i);
		m_spotLightsLocation[i].Atten.exp = getUniformLocation(Name);

		if (m_spotLightsLocation[i].colour == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].direction == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].cutoff == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.constant == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.linear == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.exp == INVALID_UNIFORM_LOCATION)
		{
			ret = false;
		}
	}

	return ret;
}

void Lighting::setWorldViewProjection(const glm::mat4 & worldViewProjection)
{
	glUniformMatrix4fv(m_worldViewProjectionLocation, 1, GL_FALSE, &worldViewProjection[0][0]);
}

void Lighting::setWorldMatrix(const glm::mat4 & worldViewProjection)
{
	glUniformMatrix4fv(m_worldMatrixLocation, 1, GL_FALSE, &worldViewProjection[0][0]);
}

void Lighting::setTextureID(unsigned int textureID)
{
	glUniform1i(m_samplerLocation, textureID);
}

void Lighting::setDirectionalLight(const DirectionalLight & light)
{
	glUniform3f(m_dirLightLocation.colour, light.colour.x, light.colour.y, light.colour.z);
	glUniform1f(m_dirLightLocation.ambientIntensity, light.ambientIntensity);
	glm::vec3 direction = light.direction;
	glm::normalize(direction);
	glUniform3f(m_dirLightLocation.direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.diffuseIntensity, light.diffuseIntensity);
}

void Lighting::setEyeWorldPos(const glm::vec3 & EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void Lighting::setMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void Lighting::setMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void Lighting::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) 
	{
		glUniform3f(m_pointLightsLocation[i].colour, pLights[i].colour.x, pLights[i].colour.y, pLights[i].colour.z);
		glUniform1f(m_pointLightsLocation[i].ambientIntensity, pLights[i].ambientIntensity);
		glUniform1f(m_pointLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].position, pLights[i].position.x, pLights[i].position.y, pLights[i].position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.constant, pLights[i].attenuation.constant);
		glUniform1f(m_pointLightsLocation[i].Atten.linear, pLights[i].attenuation.linear);
		glUniform1f(m_pointLightsLocation[i].Atten.exp, pLights[i].attenuation.exp);
	}
}

void Lighting::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(m_numSpotLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_spotLightsLocation[i].colour, pLights[i].colour.x, pLights[i].colour.y, pLights[i].colour.z);
		glUniform1f(m_spotLightsLocation[i].ambientIntensity, pLights[i].ambientIntensity);
		glUniform1f(m_spotLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].position, pLights[i].position.x, pLights[i].position.y, pLights[i].position.z);
		glm::vec3 Direction = pLights[i].direction;
		glm::normalize(Direction);
		glUniform3f(m_spotLightsLocation[i].direction, Direction.x, Direction.y, Direction.z);
		glUniform1f(m_spotLightsLocation[i].cutoff, cosf(((pLights[i].cutoff * 3.14159265359) / 180.0f)));
		glUniform1f(m_spotLightsLocation[i].Atten.constant, pLights[i].attenuation.constant);
		glUniform1f(m_spotLightsLocation[i].Atten.linear, pLights[i].attenuation.linear);
		glUniform1f(m_spotLightsLocation[i].Atten.exp, pLights[i].attenuation.exp);
	}
}



