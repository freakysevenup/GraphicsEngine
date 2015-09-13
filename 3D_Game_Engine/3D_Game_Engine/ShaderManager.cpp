#include "ShaderManager.h"
#include <stdio.h>
#include <string>
#include <fstream>


bool readFileToBuffer(std::string filePath, std::string & buffer);

ShaderManager::ShaderManager() :
m_shaderProgram(0)
{
}


ShaderManager::~ShaderManager()
{
	// Delete the intermediate shader objects that have been added to the program
	// The list will only contain something if shaders were compiled but the object itself
	// was destroyed prior to linking.
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
	{
		glDeleteShader(*it);
	}

	if (m_shaderProgram != 0)
	{
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}
}

bool ShaderManager::init()
{
	m_shaderProgram = glCreateProgram();

	if (m_shaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program.");
		return false;
	}
	return true;
}

bool ShaderManager::addShader(GLenum shaderType, const char * filePath)
{
	std::string s;

	if (!readFileToBuffer(filePath, s))
	{
		return false;
	}

	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n', shaderType");
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	m_shaderObjList.push_back(shaderObj);

	const GLchar * p[1];
	p[0] = s.c_str();
	GLint lengths[1] = { (GLint)s.size() };

	glShaderSource(shaderObj, 1, p, lengths);

	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling '%s': '%s'\n", filePath, infoLog);
		return false;
	}

	glBindAttribLocation(m_shaderProgram, 0, "Normal");

	glAttachShader(m_shaderProgram, shaderObj);
	
	return true;
}

bool ShaderManager::finalize()
{
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(m_shaderProgram);

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0)
	{
		glGetProgramInfoLog(m_shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: 's%'\n", errorLog);
		return false;
	}

	glValidateProgram(m_shaderProgram);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: 's%'\n", errorLog);
		//return false;
	}

	//Delete the intermediate shader objects that have been added to the program
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("OpenGL error in $s:%d: 0x%x\n", __FILE__, __LINE__, error);
		exit(0);
	}

	return error;
}

void ShaderManager::enable()
{
	glUseProgram(m_shaderProgram);
}

void ShaderManager::disable()
{
	glUseProgram(0);
}

GLint ShaderManager::getUniformLocation(const char * pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProgram, pUniformName);

	if (Location == 0xFFFFFFFF) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint ShaderManager::getProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(m_shaderProgram, param, &ret);
	return ret;
}

bool readFileToBuffer(std::string filePath, std::string & buffer)
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