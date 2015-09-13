#pragma once
#include "Vertex.h"
#include <string>
#include <GL/glew.h>
#include <vector>

struct Light {
	glm::vec3 position;
	glm::vec3 intensities; //a.k.a. the color of the light
};

class Mesh
{
public:

	Mesh();
	Mesh(Vertex * vertices, unsigned int numVertices, unsigned int * indices, unsigned int numIndices);
	Mesh(std::vector<Vertex> & vertices, unsigned int numVertices, bool skyBox = false);

	~Mesh();

	void draw();
	void drawModel();

private:

	enum BufferObjects
	{
		POSITION_VB,
		TEXCOORDS_VB,
		NORMALS_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject = 0;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount = 0;


};

