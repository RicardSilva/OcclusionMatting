#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <vector>
#include "vec2.h"
#include "vec3.h"

#define VERTICES 0
#define NORMALS 1
#define TEXCOORDS 2
#define TOLERANCE 1.0e-6

struct Vertex {
	GLfloat x, y, z, w;
	Vertex() {}
	Vertex(const vec3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = 1;
	}

	friend bool operator==(const Vertex& v1, const Vertex& v2) {
		return (fabs(v1.x - v2.x) < TOLERANCE
			&&  fabs(v1.y - v2.y) < TOLERANCE
			&&  fabs(v1.z - v2.z) < TOLERANCE
			&&  fabs(v1.w - v2.w) < TOLERANCE);
	}
	friend bool operator!=(const Vertex& v1, const Vertex& v2) {
		return !(v1 == v2);
	}
};

struct Texcoord {
	GLfloat u, v;
	Texcoord() {}
	Texcoord(const vec2& w) {
		u = w.x;
		v = w.y;
	}
};

struct Normal {
	GLfloat nx, ny, nz;
	Normal() {}
	Normal(const vec3& v) {
		nx = v.x;
		ny = v.y;
		nz = v.z;
	}
};
struct Material
{
	Material()
	{
		name;
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0;
	}

	std::string name;

	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ns;
	float Ni;
	float d;
	int illum;
	// Ambient Texture Map
	std::string map_Ka;
	// Diffuse Texture Map
	std::string map_Kd;
	// Specular Texture Map
	std::string map_Ks;
	// Specular Hightlight Map
	std::string map_Ns;
	// Alpha Texture Map
	std::string map_d;
	// Bump Map
	std::string map_bump;
};

struct Mesh
{
	std::string MeshName;

	std::vector <Vertex> Positions;
	std::vector <Normal> Normals;
	std::vector <Texcoord> Texcoords;
	std::vector<unsigned int> Indices;

	Material MeshMaterial;


	GLuint VaoId;
	GLuint VboVertices, VboTexcoords, VboNormals, VboIndices;
	Mesh() {}
	Mesh(std::vector<Vertex>& positions,
		std::vector <Normal>& normals,
		std::vector <Texcoord>& texcoords,
		std::vector<unsigned int>& indices)
	{
		Positions = positions;
		Normals = normals;
		Texcoords = texcoords;
		Indices = indices;
		CreateBufferObjects();
	}
	~Mesh() {
		DestroyBufferObjects();
	}


public:
	void draw() {
		glBindVertexArray(VaoId);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Positions.size());
		//glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, &Indices[0]);
		glBindVertexArray(0);
	}
private:
	void CreateBufferObjects()
	{

		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);

		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Vertex), &Positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);


		glGenBuffers(1, &VboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
		glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMALS);
		glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);

		glGenBuffers(1, &VboTexcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
		glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);

		glGenBuffers(1, &VboIndices);
		glBindBuffer(GL_ARRAY_BUFFER, VboIndices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	void DestroyBufferObjects()
	{
		glBindVertexArray(VaoId);
		glDisableVertexAttribArray(VERTICES);
		glDisableVertexAttribArray(TEXCOORDS);
		glDisableVertexAttribArray(NORMALS);
		glDeleteBuffers(1, &VboVertices);
		glDeleteBuffers(1, &VboTexcoords);
		glDeleteBuffers(1, &VboNormals);
		glDeleteBuffers(1, &VboIndices);
		glDeleteVertexArrays(1, &VaoId);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindVertexArray(0);
	}
};



struct ObjModel {
	std::vector<Mesh*> meshes;

	ObjModel() {
		meshes = std::vector<Mesh*>();
	}
};