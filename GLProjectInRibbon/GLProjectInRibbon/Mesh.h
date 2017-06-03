#pragma once
#ifndef MESH_H
#define MESH_H
//Std.Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
//GL Includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//Assimp Includes
#ifndef ASSIMP_H
#define ASSIMP_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#include "Shader.h"
#include "Color.h"

struct Vertex
{
	Vertex() {};
	Vertex(glm::vec3 & position, glm::vec3 & normal, glm::vec2 & texCoords) :
		Position(position), Normal(normal), TexCoords(texCoords) {};
	~Vertex() {};

	//Position
	glm::vec3 Position;
	//Normal
	glm::vec3 Normal;
	//TexCoords
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:
	/* Mesh Data */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	Color color;

	//Mesh Simplification Operator
	bool deleted;

	/* Functions */
	//Constructor
	Mesh();
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	~Mesh();
	//Render the mesh
	void Draw(Shader shader);

public:
	void SetDelete();
	bool isDeleted();

private:
	/* Render data */
	GLuint VAO, VBO, EBO;
	

	/* Functions  */
	//Initialize all the buffer objects / arrays
public:
	void setupMesh();
	void resetupMesh();
};

#endif