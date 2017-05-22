#pragma once
#include <iostream>
#include <map>
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character{
	GLuint TextureID;    //ID handle of the glyph texture
	glm::ivec2 Size;     //Size of glyph
	glm::ivec2 Bearing;  //Offset from baseline to left/top of glyph
	GLuint Advance;      //Horizontal offset to advance to next glyph
};

class GLText
{
public:
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
public:
	GLText();
	~GLText();
	void InitFreeType();
	void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

