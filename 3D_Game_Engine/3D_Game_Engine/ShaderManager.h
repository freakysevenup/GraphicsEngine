#pragma once
#include <list>
#include <GL/glew.h>

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class ShaderManager
{
public:
	ShaderManager();
	virtual ~ShaderManager();
	virtual bool init();
	void enable();
	void disable();

protected:

	bool addShader(GLenum shaderType, const char * filePath);
	bool finalize();
	GLint getUniformLocation(const char * pUniformName);
	GLint getProgramParam(GLint param);

	GLuint m_shaderProgram;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

