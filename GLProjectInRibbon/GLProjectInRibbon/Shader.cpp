#include "stdafx.h"
#include "Shader.h"


Shader::Shader()
{
}


Shader::~Shader()
{
}

Shader::Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
{
	//Retrive code from filepath
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFIle;
	//ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFIle.exceptions(ifstream::badbit);

	try
	{
		//Open File
		vShaderFile.open(vertexSourcePath);
		fShaderFIle.open(fragmentSourcePath);
		stringstream vShaderStream, fShaderStream;
		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFIle.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFIle.close();
		//Convert stream into GLchar array
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	//Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//Shader program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	//Print Linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	//Delete the shaders as they're linked into out program now and no longer used
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

