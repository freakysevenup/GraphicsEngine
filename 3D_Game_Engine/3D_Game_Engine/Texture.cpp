#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture() :
m_texture(0)
{

}

Texture::Texture(const std::string & fileName)
{
	//stb image will return the width, height and numberOfComponents(if relevant) for each image
	m_filePath = fileName;


	int width, height, numComponents;

	unsigned char * imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Texture failed to load : " << fileName << std::endl;
	}

	// Generate the buffer for our texture data
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Specify more information about your texture if you wish to
	// GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_S are for 
	// defineing what happens if you read outside the texture pixel width or height
	// In this case because GL_REPEAT is defined, it will go back to the beginning
	// and start there. ie if the texture is 10 pixels wide and I'm looking for the 11th pixel
	// it will return the pixel 1 instead
	// could also use GL_CLAMP, but that does not support wrapping

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLICATE_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLICATE_BORDER);

	// What happens if the texture takes up more pixels than I specified,
	// This handles determining what to do when interpolating or extrapolating. 
	// ie if the texture is rotating then the image will not be exactly 10 pixels
	// wide and 10 pixels high, the following parameter definitions will produce the most
	// accurate image they can using linear interpolation (because GL_LINEAR is defined)
	// GL_TEXTURE_MIN_FILTER handles minification, and GL_TEXTURE_MAG_FILTER
	// handles magnification. Could also use GL_NEAREST, however it creates pixelation
	// unlike GL_LINEAR

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	m_width = width;
	m_height = height;
	m_numComponents = numComponents;

	stbi_image_free(imageData);

}

Texture::Texture(GLenum textureTarget, const std::string & filePath)
{
	//stb image will return the width, height and numberOfComponents(if relevant) for each image
	m_filePath = filePath;
	m_textureTarget = textureTarget;
}

Texture::~Texture(void)
{
	// Destroy the texture when it no longer in use.
	// better to create a texture cache so the textures can be reused if needed though
	glDeleteTextures(1, &m_texture);
}

bool Texture::loadModel()
{
	int width, height, numComponents;

	unsigned char * imageData = stbi_load(m_filePath.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Texture failed to load : " << m_filePath << std::endl;
		return false;
	}

	// Generate the buffer for our texture data
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Specify more information about your texture if you wish to
	// GL_TEXTURE_WRAP_T and GL_TEXTURE_WRAP_S are for 
	// defineing what happens if you read outside the texture pixel width or height
	// In this case because GL_REPEAT is defined, it will go back to the beginning
	// and start there. ie if the texture is 10 pixels wide and I'm looking for the 11th pixel
	// it will return the pixel 1 instead
	// could also use GL_CLAMP, but that does not support wrapping

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLICATE_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLICATE_BORDER);

	// What happens if the texture takes up more pixels than I specified,
	// This handles determining what to do when interpolating or extrapolating. 
	// ie if the texture is rotating then the image will not be exactly 10 pixels
	// wide and 10 pixels high, the following parameter definitions will produce the most
	// accurate image they can using linear interpolation (because GL_LINEAR is defined)
	// GL_TEXTURE_MIN_FILTER handles minification, and GL_TEXTURE_MAG_FILTER
	// handles magnification. Could also use GL_NEAREST, however it creates pixelation
	// unlike GL_LINEAR

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	m_width = width;
	m_height = height;
	m_numComponents = numComponents;

	stbi_image_free(imageData);

	return true;
}

void Texture::bind2D(unsigned int index)
{
	// GlTextures are not less than zero and they do not store more than
	// 32 textures, asserting here ensures that the index passed
	// does not fall out of that range
	assert(index >= 0 && index <= 31);

	// Gl Textures are ordered sequentially
	// so it will select the appropriate texture
	glActiveTexture(GL_TEXTURE0 + index);

	glBindTexture(GL_TEXTURE_2D, m_texture);

}

void Texture::bindModel(GLenum textureUnit)
{

	glActiveTexture(textureUnit);

	glBindTexture(m_textureTarget, m_texture);

}

void Texture::createCubeMap(
	const char * front,
	const char * back,
	const char * top,
	const char * bottom,
	const char * left,
	const char * right)
{
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_texture);

	// load each image and copy into a side of the cube-map texture

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, back);

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, front);

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);

		load_cube_map_side(m_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

bool Texture::load_cube_map_side(
	GLuint texture, GLenum side_target, const char* file_name
	) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int force_channels = 4;
	unsigned char*  image_data = stbi_load(
		file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}

	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
			);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(
		side_target,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
		);
	free(image_data);
	return true;
}

void Texture::bindCube(unsigned int index)
{
	// GlTextures are not less than zero and they do not store more than
	// 32 textures, asserting here ensures that the index passed
	// does not fall out of that range
	assert(index >= 0 && index <= 31);

	// Gl Textures are ordered sequentially
	// so it will select the appropriate texture
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

}