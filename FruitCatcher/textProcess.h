#pragma once

#include <SOIL/SOIL.h>
#include <SDL/SDL_mixer.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <map>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <time.h>
#include "Game.h"

using namespace glm;

#define FONTSIZE 40
#define FONTNAME "cs_regular.ttf"
#define NUM_BUTTON 3

struct myCharacter {
	GLuint TextureID; // ID handle of the glyph texture
	ivec2 Size; // Size of glyph
	ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance; // Offset to advance to next glyph
};

class textProcess :
	public Engine::Game
{
public:
	textProcess();
	~textProcess();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	void InitText();
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);

private:
	GLuint VAOtext, VBOtext, EBOtext, Texturetext,programText;
	map<GLchar, myCharacter> Characters;

	

};

