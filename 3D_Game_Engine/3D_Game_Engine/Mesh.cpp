#include "Mesh.h"
#include <vector>
#include <iostream>

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> & vertices, unsigned int numVertices, bool skyBox /* = false */)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> cubeTexCoords;

	switch (skyBox)
	{
	case true:
		m_drawCount = numVertices;

		// Create Space for an Vertex array
		glGenVertexArrays(1, &m_vertexArrayObject);

		// Binding the vertex array object make it so that any operation that effects a vertex array object, will affect this vertex array object
		glBindVertexArray(m_vertexArrayObject);


		/////////////////ADDED IN TUT 4/////////////////////////////////////////

		positions.reserve(numVertices);
		cubeTexCoords.reserve(numVertices);
		normals.reserve(numVertices);

		for (unsigned int i = 0; i < numVertices; i++)
		{
			positions.push_back(*vertices[i].getPosition());
			cubeTexCoords.push_back(*vertices[i].getSkyBoxTexCoords());
			normals.push_back(*vertices[i].getNormals());
		}


		// Generate all the buffers that we need so we can write to them.
		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);


		//////---POSITIONS---/////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(positions[0])), // size of array,
			&positions[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(0);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			0, // Which attribute array its being told how to interpret
			3, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute

		/////////////////////////////////////////////////////////////////////////////////////////////////

		///////---TEXTURE COORDINATES---/////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORDS_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(cubeTexCoords[0])), // size of array,
			&cubeTexCoords[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(1);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			1, // Which attribute array its being told how to interpret
			3, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute

		/////////////////////////////////////////////////////////////////////////////////////////


		////////---NORMALS---////////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMALS_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(normals[0])), // size of array,
			&normals[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(2);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			2, // Which attribute array its being told how to interpret
			3, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute


		//////// Binding the buffer to the GPU
		//////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		//////// GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		//////// Store all our vertex data into the buffer (array)
		//////glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertices.size()), &vertices[0], GL_STATIC_DRAW);


		// This stops the GPU from affecting the vertex array object that we previously bound to the gpu, effectively unbinds it.
		glBindVertexArray(0);

		/////////////////////////////////////////////////////////////////////////////////////////
		break;


	case false:
		m_drawCount = numVertices;

		// Create Space for an Vertex array
		glGenVertexArrays(1, &m_vertexArrayObject);

		// Binding the vertex array object make it so that any operation that effects a vertex array object, will affect this vertex array object
		glBindVertexArray(m_vertexArrayObject);


		/////////////////ADDED IN TUT 4/////////////////////////////////////////

		positions.reserve(numVertices);
		textureCoords.reserve(numVertices);
		normals.reserve(numVertices);

		for (unsigned int i = 0; i < numVertices; i++)
		{
			positions.push_back(*vertices[i].getPosition());
			textureCoords.push_back(*vertices[i].getTextureCoords());
			normals.push_back(*vertices[i].getNormals());
		}


		// Generate all the buffers that we need so we can write to them.
		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);


		//////---POSITIONS---/////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(positions[0])), // size of array,
			&positions[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(0);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			0, // Which attribute array its being told how to interpret
			3, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute

		/////////////////////////////////////////////////////////////////////////////////////////////////

		///////---TEXTURE COORDINATES---/////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORDS_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(textureCoords[0])), // size of array,
			&textureCoords[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(1);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			1, // Which attribute array its being told how to interpret
			2, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute

		/////////////////////////////////////////////////////////////////////////////////////////


		////////---NORMALS---////////////////////////////////////////////////////////////////////

		// Binding the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMALS_VB]); // GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		// Store all our vertex data into the buffer (array)
		glBufferData
			(
			GL_ARRAY_BUFFER, // think of this as an array,
			(numVertices * sizeof(normals[0])), // size of array,
			&normals[0], // the array of vertices we want to store,
			GL_STATIC_DRAW // GL_STATIC_DRAW -- the data will not be changed, it doesn't need to be accessed repeatedly
			);

		// This tells the GPU that there are going to be attributes for these Vertices
		glEnableVertexAttribArray(2);

		// This tells the GPU how to actually read each attribute into an array
		glVertexAttribPointer(
			2, // Which attribute array its being told how to interpret
			3, // how many pieces of data in that attribute
			GL_FLOAT, // what kind of data are these attributes
			GL_FALSE, // Tell the GPU if you want to Normalize it
			0, // Tell the GPU how many pieces of data to skip between to find the next attribute
			0); // where to start to find the first attribute


		//////// Binding the buffer to the GPU
		//////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		//////// GL_ARRAY_BUFFER -- tells the GPU to interpret this buffer of data as an array -> this array (m_vertexArrayBuffers)

		//////// Store all our vertex data into the buffer (array)
		//////glBufferData(GL_ELEMENT_ARRAY_BUFFER, (vertices.size()), &vertices[0], GL_STATIC_DRAW);


		// This stops the GPU from affecting the vertex array object that we previously bound to the gpu, effectively unbinds it.
		glBindVertexArray(0);

		/////////////////////////////////////////////////////////////////////////////////////////
		break;
	}

}

Mesh::~Mesh()
{
	// Delete the vertex array to free the space allocated to it
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::draw()
{

	glBindVertexArray(m_vertexArrayObject);

	// mode - how to draw, (triangles, lines, points), first, count
	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	//glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

