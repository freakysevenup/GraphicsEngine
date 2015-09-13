#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>

enum class SHADER_TYPE
{
	VERTEX,
	FRAGMENT,
	GEOMETRY
};

class ShaderNova
{
public:
	ShaderNova(void);

	static bool readFileToBuffer(std::string filePath, std::string & buffer);

	void compileShadersFromFile(const std::vector<std::string> & shaderFilePaths);
	void linkShaders();
	void addAttributes(const std::string & attributeName);
	GLint getUniformLocation(const std::string & uniformName);
	void startUse();
	void stopUse();
	void dispose();

	std::string vertSource = "";
	std::string fragSource = "";
	std::string geometrySource = "";

private:

	void programSetup(std::vector<SHADER_TYPE> types);
	void programSetup(SHADER_TYPE type);

	void compileShaders(const char * source, GLuint _id, const std::string & shaderName);
	GLuint m_program;
	GLuint m_vertexShader;
	GLuint m_fragShader;
	GLuint m_geoShader;
	int m_numAttributes;

};

