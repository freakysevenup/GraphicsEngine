#include "ShaderNova.h"
#include <fstream>
#include <iostream>
#include <vector>

ShaderNova::ShaderNova():
m_numAttributes(0),
m_program(0),
m_vertexShader(0),
m_fragShader(0)
{

}

bool ShaderNova::readFileToBuffer(std::string filePath, std::string & buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//To figure out the size of a file we want to use a method to find the 
	//end of the file. this will tell us how many bytes the file is

	//Seek to the end of the file
	file.seekg(0, std::ios::end);

	//Get the file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//at the top of every file is a file header it is information about a file, like
	//when it was last edited for example. You will not normally see this information
	//so if we don't want to read those bytes we can stop that from happening
	//This is not necessary however it is better safe than sorry.

	fileSize -= file.tellg();

	//resize the buffer to the size of the file we are reading
	buffer.resize(fileSize);

	//&(buffer[0]) is like saying what is the address of the first element of the buffer array, 
	//that is where we want to start reading from.
	file.read((char *)&(buffer[0]), fileSize);
	file.close();

	//The file was successfully opened and stored in the buffer
	return true;
}

void ShaderNova::programSetup(std::vector<SHADER_TYPE> types)
{
	m_program = glCreateProgram();

	for (int i = 0; i < types.size(); i++)
	{
		switch (types[i])
		{
		case SHADER_TYPE::VERTEX:
			m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case SHADER_TYPE::FRAGMENT:
			m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case SHADER_TYPE::GEOMETRY:
			m_geoShader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
			break;
		}
	}
}


void ShaderNova::compileShaders(const char * source, GLuint _id, const std::string & shaderName)
{
	glShaderSource(_id, 1, &source, nullptr);

	glCompileShader(_id);

	GLint success = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

		//The max length includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(_id, maxLength, &maxLength, &errorLog[0]);

		//Provide the infolog in whatever manor you deem best.
		//Exit with Failure
		glDeleteShader(_id); //don't leak the shader

		std::printf("%s\n", &errorLog[0]);

	}
}

void ShaderNova::compileShadersFromFile(const std::vector<std::string> & shaderFilePaths)
{
	std::vector<SHADER_TYPE> type;

	int i = 0;

	switch (shaderFilePaths.size())
	{
	case 2:
		ShaderNova::readFileToBuffer(shaderFilePaths[i], vertSource);
		ShaderNova::readFileToBuffer(shaderFilePaths[i + 1], fragSource);

		type.push_back(SHADER_TYPE::VERTEX);
		type.push_back(SHADER_TYPE::FRAGMENT);

		programSetup(type);

		compileShaders(vertSource.c_str(), m_vertexShader, shaderFilePaths[i]);
		compileShaders(fragSource.c_str(), m_fragShader, shaderFilePaths[i + 1]);
		break;
	case 3:
		ShaderNova::readFileToBuffer(shaderFilePaths[i], vertSource);
		ShaderNova::readFileToBuffer(shaderFilePaths[i + 1], fragSource);
		ShaderNova::readFileToBuffer(shaderFilePaths[i + 2], geometrySource);

		type.push_back(SHADER_TYPE::VERTEX);
		type.push_back(SHADER_TYPE::FRAGMENT);
		type.push_back(SHADER_TYPE::GEOMETRY);

		programSetup(type);

		compileShaders(vertSource.c_str(), m_vertexShader, shaderFilePaths[i]);
		compileShaders(fragSource.c_str(), m_fragShader, shaderFilePaths[i + 1]);
		compileShaders(geometrySource.c_str(), m_geoShader, shaderFilePaths[i + 2]);
		break;
	default:
		break;
	}

	vertSource = "";
	fragSource = "";
	geometrySource = "";
}

void ShaderNova::linkShaders()
{
	//Attach our shaders to our program
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragShader);

	//Link our program
	glLinkProgram(m_program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(m_program);
		//Don't leak shaders either.
		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragShader);

		//Use the infoLog as you see fit
		std::printf("%s\n", &errorLog[0]);
	}

	//Always detach shaders after a successful link
	glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragShader);

	//Don't leak shaders either.
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragShader);

	glGetError();
}

void ShaderNova::addAttributes(const std::string & attributeName)
{
	glBindAttribLocation(m_program, m_numAttributes++, attributeName.c_str());
}

GLint ShaderNova::getUniformLocation(const std::string & uniformName)
{
	GLint location = glGetUniformLocation(m_program, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
		std::cout << "location = GL_INVALID_INDEX  " + uniformName << std::endl;
		//exit(0);
	return location;
}

void ShaderNova::startUse()
{
	glUseProgram(m_program);
	for (int i = 0; i < m_numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void ShaderNova::stopUse()
{
	glUseProgram(0);
	for (int i = 0; i < m_numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

void ShaderNova::dispose()
{
	if (m_program)
	{
		glDeleteProgram(m_program);
	}
}