#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#include <glew.h>

class Shader
{
public:
	Shader();
	~Shader();
	//Program ID
	GLuint Program;
	//Constructor read and create shader
	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
	//Use Program
	void Use();
};


#endif
