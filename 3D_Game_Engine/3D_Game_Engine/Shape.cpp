#include "Shape.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <iostream>


Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::draw()
{
	this->m_mesh.draw();
}

Shape Shape::createShape(
	ShapeType type, 
	float size, 
	float texturing /* = 2.0f */, 
	int resolution /* = 16 */,
	float width /* = 1.0f */, 
	float height /* = 1.0f */,
	float depth /* = 1.0f */)
{
	this->setSize(size);
	this->setResolution(resolution);

	switch (type)
	{
	case SPHERE:
		this->setVertices(createSphere(size, resolution, true));
		break;
	case CUBE:
		this->setVertices(createCube(size, texturing));
		break;
	case PYRAMID:
		this->setVertices(createPyramid(size, texturing));
		break;
	case PLANE:
		this->setVertices(createPlane(size, texturing));
		break;
	case TEXTURED_CUBE:
		this->setVertices(createSkyCube(size, texturing));
		break;
	case CUBOID:
		this->setVertices(createCuboid(width, height, depth, texturing));
		break;
	default:

		break;
	}

	this->setMesh(Mesh(this->m_vertices, this->m_vertices.size()));

	return *(this);
}


std::vector<Vertex> Shape::createPyramid(float size, float texturing)
{
	auto tex = texturing / 2.0f;

	std::vector<Vertex> vertices;

	/* Front */
	glm::vec3 frontNormal = computePlaneNormal(
		glm::vec3(-size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(size, -size, -size));

	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(0.0f, size, 0.0f), glm::vec2(tex, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), frontNormal);

	/* Right */
	glm::vec3 rightNormal = computePlaneNormal(
		glm::vec3(size, -size, -size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(size, -size, size));

	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(0.0f, size, 0.0f), glm::vec2(tex, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(0.0f), rightNormal);

	/* Back */
	glm::vec3 backNormal = computePlaneNormal(
		glm::vec3(-size, -size, size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(size, -size, size));

	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(0.0f), -backNormal);
	vertices.emplace_back(glm::vec3(0.0f, size, 0.0f), glm::vec2(tex, texturing), -backNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), -backNormal);

	/* Left */
	glm::vec3 leftNormal = computePlaneNormal(
		glm::vec3(-size, -size, size),
		glm::vec3(0.0f, size, 0.0f),
		glm::vec3(-size));

	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), leftNormal);
	vertices.emplace_back(glm::vec3(0.0f, size, 0.0f), glm::vec2(tex, texturing), leftNormal);
	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), leftNormal);

	/* Bottom 1 */
	glm::vec3 bottomNormal = computePlaneNormal(
		glm::vec3(size, -size, -size),
		glm::vec3(size, -size, size),
		glm::vec3(-size));


	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), bottomNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), bottomNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(tex, texturing), bottomNormal);

	/* Bottom 2 */


	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), bottomNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(tex, texturing), bottomNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), bottomNormal);

	glCullFace(GL_BACK);

	return vertices;
}

std::vector<Vertex> Shape::createCube(float size, float texturing)
{
	auto tex = texturing / 2.0f;

	std::vector<Vertex> vertices;


	glm::vec3 frontNormal = computePlaneNormal(
		glm::vec3(-size, size, -size),
		glm::vec3(size, -size, -size),
		glm::vec3(-size));

	glm::vec3 topNormal = computePlaneNormal(
		glm::vec3(-size, size, -size),
		glm::vec3(size),
		glm::vec3(-size, size, size));

	glm::vec3 rightNormal = computePlaneNormal(
		glm::vec3(-size, -size, size),
		glm::vec3(-size),
		glm::vec3(-size, size, size));

	/* FRONT */

	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(-size), glm::vec2(tex, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(tex, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), frontNormal);

	/* LEFT */

	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-size), glm::vec2(tex, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(tex, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), -rightNormal);

	/* RIGHT */

	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(tex, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(tex, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), rightNormal);

	/* BACK */

	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(tex, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(tex, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), -frontNormal);

	/* BOTTOM */

	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(tex, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(tex, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), -topNormal);

	/* TOP */

	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(tex, texturing), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(tex, texturing), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(texturing, 0.0f), topNormal);

	glCullFace(GL_FRONT);

	return vertices;
}

std::vector<Vertex> Shape::createCuboid(float width, float height, float depth, float texturing)
{
	auto tex = texturing / 2.0f;

	std::vector<Vertex> vertices;

	glm::vec3 frontNormal = computePlaneNormal(
		glm::vec3(-width, height, -depth),
		glm::vec3(width, -height, -depth),
		glm::vec3(-width, height, depth));

	glm::vec3 topNormal = computePlaneNormal(
		glm::vec3(-width, height, -depth),
		glm::vec3(width, height, depth),
		glm::vec3(-width, height, depth));

	glm::vec3 rightNormal = computePlaneNormal(
		glm::vec3(-width, -height, depth),
		glm::vec3(-width, height, depth),
		glm::vec3(-width, height, depth));

	/* Front */

	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(-width, -height, -depth), glm::vec2(tex, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(texturing, 0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(width, height, -depth), glm::vec2(tex, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(texturing, 0.0f), frontNormal);

	/* Right */

	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-width, -height, -depth), glm::vec2(tex, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(texturing, 0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-width, height, depth), glm::vec2(tex, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(texturing, 0.0f), -rightNormal);

	/* Left */

	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(width, -height, depth), glm::vec2(tex, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(texturing, 0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(width, height, -depth), glm::vec2(tex, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(texturing, 0.0f), rightNormal);

	/* Back */

	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(-width, height, depth), glm::vec2(tex, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(texturing, 0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(width, -height, depth), glm::vec2(tex, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(texturing, 0.0f), -frontNormal);

	/* Top */

	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(width, height, -depth), glm::vec2(tex, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(texturing, 0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(width, height, depth), glm::vec2(0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(-width, height, depth), glm::vec2(tex, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(-width, height, -depth), glm::vec2(texturing, 0.0f), -topNormal);

	/* Bottom */

	vertices.emplace_back(glm::vec3(-width, -height, -depth), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(tex, texturing), topNormal);
	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(texturing, 0.0f), topNormal);
	vertices.emplace_back(glm::vec3(width, -height, -depth), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-width, -height, depth), glm::vec2(tex, texturing), topNormal);
	vertices.emplace_back(glm::vec3(width, -height, depth), glm::vec2(texturing, 0.0f), topNormal);

	glCullFace(GL_FRONT);

	return vertices;
}

std::vector<Vertex> Shape::createSphere(float Radius, int Resolution, bool InvertNormals)
{
	std::vector<Vertex> s_vertices;

	glm::vec3 *Vertices, *Normals;
	glm::vec2 *TexCoords;

	int TrianglesCount;

	if (Resolution < 16) Resolution = 16;

	if (Resolution % 4) Resolution += 4 - Resolution % 4;

	TrianglesCount = Resolution * (Resolution / 4 - 1) * 4 + Resolution * 2;

	Vertices = new glm::vec3[TrianglesCount * 3];
	TexCoords = new glm::vec2[TrianglesCount * 3];
	Normals = new glm::vec3[TrianglesCount * 3];
	s_vertices.resize(TrianglesCount * 3);

	float angle = (float)3.14159265359 * 2.0f / Resolution;

	glm::vec3 a, b, c, d;
	glm::vec2 tca, tcb, tcc, tcd;

	int i = 0;

	float r = (float)Resolution, r4 = (float)Resolution / 2.0f;

	for (int y = 0; y < Resolution / 4; y++)
	{
		for (int xz = 0; xz < Resolution; xz++)
		{
			if (y < Resolution / 4 - 1)
			{
				a = glm::vec3(-sin(angle * (xz + 0)) * cos(angle * (y + 0)), sin(angle * (y + 0)), -cos(angle * (xz + 0)) * cos(angle * (y + 0)));
				b = glm::vec3(-sin(angle * (xz + 1)) * cos(angle * (y + 0)), sin(angle * (y + 0)), -cos(angle * (xz + 1)) * cos(angle * (y + 0)));
				c = glm::vec3(-sin(angle * (xz + 1)) * cos(angle * (y + 1)), sin(angle * (y + 1)), -cos(angle * (xz + 1)) * cos(angle * (y + 1)));
				d = glm::vec3(-sin(angle * (xz + 0)) * cos(angle * (y + 1)), sin(angle * (y + 1)), -cos(angle * (xz + 0)) * cos(angle * (y + 1)));

				tca = TexCoords[i] = glm::vec2((xz + 0) / r, 0.5f + (y + 0) / r4);
				tcb = TexCoords[i] = glm::vec2((xz + 1) / r, 0.5f + (y + 0) / r4);
				tcc = TexCoords[i] = glm::vec2((xz + 1) / r, 0.5f + (y + 1) / r4);
				tcd = TexCoords[i] = glm::vec2((xz + 0) / r, 0.5f + (y + 1) / r4);

				TexCoords[i] = tca; Normals[i] = a; Vertices[i++] = a * Radius;
				TexCoords[i] = tcb; Normals[i] = b; Vertices[i++] = b * Radius;
				TexCoords[i] = tcc; Normals[i] = c; Vertices[i++] = c * Radius;

				TexCoords[i] = tcc; Normals[i] = c; Vertices[i++] = c * Radius;
				TexCoords[i] = tcd; Normals[i] = d; Vertices[i++] = d * Radius;
				TexCoords[i] = tca; Normals[i] = a; Vertices[i++] = a * Radius;

				a.y = -a.y;
				b.y = -b.y;
				c.y = -c.y;
				d.y = -d.y;

				tca.y = 1.0f - tca.y;
				tcb.y = 1.0f - tcb.y;
				tcc.y = 1.0f - tcc.y;
				tcd.y = 1.0f - tcd.y;

				TexCoords[i] = tcd; Normals[i] = d; Vertices[i++] = d * Radius;
				TexCoords[i] = tcc; Normals[i] = c; Vertices[i++] = c * Radius;
				TexCoords[i] = tcb; Normals[i] = b; Vertices[i++] = b * Radius;

				TexCoords[i] = tcb; Normals[i] = b; Vertices[i++] = b * Radius;
				TexCoords[i] = tca; Normals[i] = a; Vertices[i++] = a * Radius;
				TexCoords[i] = tcd; Normals[i] = d; Vertices[i++] = d * Radius;
			}
			else
			{
				a = glm::vec3(-sin(angle * (xz + 0)) * cos(angle * (y + 0)), sin(angle * (y + 0)), -cos(angle * (xz + 0)) * cos(angle * (y + 0)));
				b = glm::vec3(-sin(angle * (xz + 1)) * cos(angle * (y + 0)), sin(angle * (y + 0)), -cos(angle * (xz + 1)) * cos(angle * (y + 0)));
				c = glm::vec3(0.0f, 1.0f, 0.0f);

				tca = TexCoords[i] = glm::vec2((xz + 0) / r, 0.5f + (y + 0) / r4);
				tcb = TexCoords[i] = glm::vec2((xz + 1) / r, 0.5f + (y + 0) / r4);
				tcc = TexCoords[i] = glm::vec2((xz + 0.5f) / r, 1.0f);

				TexCoords[i] = tca; Normals[i] = a; Vertices[i++] = a * Radius;
				TexCoords[i] = tcb; Normals[i] = b; Vertices[i++] = b * Radius;
				TexCoords[i] = tcc; Normals[i] = c; Vertices[i++] = c * Radius;

				a.y = -a.y;
				b.y = -b.y;
				c.y = -c.y;

				tca.y = 1.0f - tca.y;
				tcb.y = 1.0f - tcb.y;
				tcc.y = 1.0f - tcc.y;

				TexCoords[i] = tca; Normals[i] = a; Vertices[i++] = a * Radius;
				TexCoords[i] = tcc; Normals[i] = c; Vertices[i++] = c * Radius;
				TexCoords[i] = tcb; Normals[i] = b; Vertices[i++] = b * Radius;
			}
		}
	}

	for (int i = 0; i < TrianglesCount * 3; i++)
	{
		s_vertices[i].setPosition(Vertices[i]);
		s_vertices[i].setNormals(Normals[i]);
		s_vertices[i].setTexCoords(TexCoords[i]);
	}

	delete[] Vertices;
	delete[] TexCoords;
	delete[] Normals;

	return s_vertices;
}

std::vector<Vertex> Shape::createPlane(float size, float texturing)
{

	// The order in which you draw the points of a triangle matter
	// drawn one way the face that gets textured will face down
	// drawn the other way the face that gets textured will face up

	auto tex = texturing / 2.0f;

	std::vector<Vertex> vertices;

	glm::vec3 normal = computePlaneNormal(
		glm::vec3(-size, 0, size), 
		glm::vec3(size, 0, -size), 
		glm::vec3(size, 0, size));

	vertices.emplace_back(glm::vec3(-size, 0, size), glm::vec2(tex, texturing), -normal);
	vertices.emplace_back(glm::vec3(size, 0, -size), glm::vec2(texturing, 0.0), -normal);
	vertices.emplace_back(glm::vec3(-size, 0, -size), glm::vec2(0.0), -normal);


	vertices.emplace_back(glm::vec3(size, 0, size), glm::vec2(0.0), -normal);
	vertices.emplace_back(glm::vec3(size, 0, -size), glm::vec2(texturing, 0.0), -normal);
	vertices.emplace_back(glm::vec3(-size, 0, size), glm::vec2(tex, texturing), -normal);

	//vertices.emplace_back(glm::vec3(-size, 0, size), glm::vec2(0.0), glm::vec3(-size, 0.0, size));
	//vertices.emplace_back(glm::vec3(size, 0, -size), glm::vec2(texturing, 0.0), glm::vec3(size, 0.0, -size));
	//vertices.emplace_back(glm::vec3(-size, 0, -size), glm::vec2(tex, texturing), glm::vec3(-size, 0.0, -size));


	//vertices.emplace_back(glm::vec3(size, 0, size), glm::vec2(0.0), glm::vec3(size, 0.0, size));
	//vertices.emplace_back(glm::vec3(size, 0, -size), glm::vec2(texturing, 0.0), glm::vec3(size, 0.0, -size));
	//vertices.emplace_back(glm::vec3(-size, 0, size), glm::vec2(tex, texturing), glm::vec3(-size, 0.0, size));

	return vertices;

}

std::vector<Vertex> Shape::createSkyCube(float size, float texturing)
{
	float val = 1.0f, tex = 0.0f;// texturing / 2.0f;

	std::vector<Vertex> vertices;

	glm::vec3 frontNormal = computePlaneNormal(
		glm::vec3(-size, size, -size),
		glm::vec3(size, -size, -size),
		glm::vec3(-size));

	glm::vec3 topNormal = computePlaneNormal(
		glm::vec3(-size, size, -size),
		glm::vec3(size),
		glm::vec3(-size, size, size));

	glm::vec3 rightNormal = computePlaneNormal(
		glm::vec3(-size, -size, size),
		glm::vec3(-size),
		glm::vec3(-size, size, size));

	glm::vec3 textureUV = glm::vec3(1.0f);

	
	/* FRONT */

	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing), frontNormal);
	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(0.0f), frontNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(0.0f, texturing), frontNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), frontNormal);

	/* LEFT */

	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(0.0f), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(0.0f, texturing), -rightNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), -rightNormal);

	/* RIGHT */

	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(0.0f, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), rightNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(0.0f, texturing), rightNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), rightNormal);

	/* BACK */

	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(0.0f, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), -frontNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(0.0f, texturing), -frontNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(texturing, 0.0f), -frontNormal);

	/* BOTTOM */

	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing), -topNormal);
	vertices.emplace_back(glm::vec3(size, size, -size), glm::vec2(0.0f, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(texturing, 0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(size), glm::vec2(0.0f), -topNormal);
	vertices.emplace_back(glm::vec3(-size, size, size), glm::vec2(0.0f, texturing), -topNormal);
	vertices.emplace_back(glm::vec3(-size, size, -size), glm::vec2(texturing, 0.0f), -topNormal);

	/* TOP */

	vertices.emplace_back(glm::vec3(-size), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(0.0f, texturing), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(texturing, 0.0f), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, -size), glm::vec2(0.0f), topNormal);
	vertices.emplace_back(glm::vec3(-size, -size, size), glm::vec2(0.0f, texturing), topNormal);
	vertices.emplace_back(glm::vec3(size, -size, size), glm::vec2(texturing, 0.0f), topNormal);


	glCullFace(GL_BACK);

	return vertices;
}

glm::vec3 Shape::computePlaneNormal(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC)
{
	glm::vec3 AB = pointB - pointA;
	glm::vec3 AC = pointC - pointA;
	return cross(AB, AC);
}