#include "AssImpModelLoader.h"
#include <assert.h>
#include <GLM/glm.hpp>
#include <iostream>


/*

Copyright 2011 Etay Meiri

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


ModelMesh::MeshEntry::MeshEntry()
{
	VB = 0;
	IB = 0;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
};

ModelMesh::MeshEntry::~MeshEntry()
{
	if (VB != 0)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != 0)
	{
		glDeleteBuffers(1, &IB);
	}
}

void ModelMesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
	const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &Indices[0], GL_STATIC_DRAW);
}

ModelMesh::ModelMesh()
{
}


ModelMesh::~ModelMesh()
{
	Clear();
}


void ModelMesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++) 
	{
		if (m_Textures[i])
		{
			delete m_Textures[i];
			m_Textures[i] = NULL;
		}
	}
}


bool ModelMesh::LoadMesh(const std::string & filePath, const std::string & imagesFilePath)
{
	static int counter;
	counter += 1;

	std::cout << "AssImp is processing model : " << counter << "\nat path : " << filePath << std::endl;

	// Release the previously loaded mesh (if it exists)
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(
		filePath.c_str(),
		aiProcess_Triangulate | 
		aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs);


	if (pScene) {
		Ret = InitFromScene(pScene, filePath, imagesFilePath);
	}
	else {
		printf("Error parsing '%s': '%s'\n", filePath.c_str(), Importer.GetErrorString());
	}

	m_counter = counter;

	return Ret;
}

bool ModelMesh::InitFromScene(const aiScene* pScene, const std::string& filePath, const std::string & imagesFilePath)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, filePath, imagesFilePath);
}

void ModelMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
		//m_vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, Indices);

	m_counter++;

	std::cout << "Model Mesh Intialized for model : " << m_counter << std::endl;
}

//bool ModelMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
//{
//	// Extract the directory part from the file name
//	std::string::size_type SlashIndex = Filename.find_last_of("/");
//	std::string Dir;
//
//	if (SlashIndex == std::string::npos) {
//		Dir = ".";
//	}
//	else if (SlashIndex == 0) {
//		Dir = "/";
//	}
//	else {
//		Dir = Filename.substr(0, SlashIndex);
//	}
//
//	bool Ret = true;
//
//	// Initialize the materials
//	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
//		const aiMaterial* pMaterial = pScene->mMaterials[i];
//
//		m_Textures[i] = NULL;
//
//		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
//			aiString Path;
//
//			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//				std::string FullPath = Dir + "/" + Path.data;
//				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());
//
//				if (!m_Textures[i]->loadModel()) {
//					printf("Error loading texture '%s'\n", FullPath.c_str());
//					delete m_Textures[i];
//					m_Textures[i] = NULL;
//					Ret = false;
//				}
//				else {
//					printf("Loaded texture '%s'\n", FullPath.c_str());
//				}
//			}
//		}
//
//		// Load a white texture in case the model does not include its own texture
//		if (!m_Textures[i]) {
//			m_Textures[i] = new Texture(GL_TEXTURE_2D, "./Assets/Textures/white.png");
//
//			Ret = m_Textures[i]->loadModel();
//		}
//	}
//
//	return Ret;
//}

bool ModelMesh::InitMaterials(const aiScene* pScene, const std::string& filePath, const std::string & imagesFilePath)
{
	bool Ret;
	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) 
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = imagesFilePath + Path.data;

				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_Textures[i]->loadModel()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i]) {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "./Assets/Textures/white.png");

			Ret = m_Textures[i]->loadModel();
		}
	}

	std::cout << "Model Materials Intialized for model : " << m_counter << std::endl;

	return Ret;
}

void ModelMesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->bindModel(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
