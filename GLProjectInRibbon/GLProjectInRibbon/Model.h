#pragma once
#ifndef MODEL_H
#define MODEL_H
//Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
//GL Includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SOIL.h>

#include "Simplify.h"

#ifndef ASSIMP_H
#define ASSIMP_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#include "Mesh.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
	/* Functions */
	//Constructor, expetcs a file path to a 3D model
	Model();
	Model(GLchar* path);
	//Draw the model, and thus all its meshes
	void Draw(Shader shader);
	~Model();
	vector<Mesh> getModelMeshes();
	
	GLint face_num; // face number
private:
	/* Model Data*/
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded; //Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once
	


	/* Functions */
	//Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void loadModel(string path);
	//Process a node in a recursive fashion. Process each individual mesh located at the node and repeats its process on its children nodes
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	//Checks all matrial textures of a given type and loads the textures if they're not loaded yet
	//The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);

public:
	//Change .obj to Mesh
	void ChangeToMesh(const Simplify & simp);
};

#endif