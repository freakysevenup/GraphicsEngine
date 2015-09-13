#pragma once
#include <string>
#include <GL/glew.h>

/*

	This texture loader use the stb_image image loader
	class

*/

class Texture
{
public:
	Texture();
	Texture(const std::string & fileName);
	Texture(GLenum textureTarget, const std::string & filePath);
	~Texture(void);

	bool loadModel();

	void bind2D(unsigned int index);
	void bindModel(unsigned int index);
	void bindCube(unsigned int index);

	void createCubeMap(
		const char * front,
		const char * back,
		const char * top,
		const char * bottom,
		const char * left,
		const char * right);

	// Getters
	const int getWidth() { return m_width; }
	const int getHeight() { return m_height; }
	const int getComponents() { return m_numComponents; }
	const std::string getFilePath() { return m_filePath; }
	const GLuint getTexID() { return m_texture; }

	void setNulls() { m_width = 0; m_height = 0; m_numComponents = 0; m_texture = 0; }

private:

	bool load_cube_map_side(
		GLuint texture,
		GLenum side_target,
		const char* file_name);

	GLuint m_texture = 0;
	GLenum m_textureTarget;
	int m_width = 0, m_height = 0, m_numComponents = 0;
	std::string m_filePath;
};

