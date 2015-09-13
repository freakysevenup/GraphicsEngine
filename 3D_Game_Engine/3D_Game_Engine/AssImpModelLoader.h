#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Texture.h"
#include "Shape.h"
#include "Vertex.h"


class ModelMesh :
	public Transform
{
public:
	ModelMesh();

	~ModelMesh();

	bool LoadMesh(const std::string& filePath, const std::string & imagesFilePath);

	void Render();

	//inline std::vector<Vertex> & getVertices() { return m_vertices; }

private:
	bool InitFromScene(const aiScene* pScene, const std::string& filePath, const std::string & imagesFilePath);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& filePath, const std::string & imagesFilePath);
	void Clear();

	

#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices,
			const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;

	};


	//std::vector<Vertex> m_vertices;

	int m_counter = 0;
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

};