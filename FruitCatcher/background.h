#pragma once
#ifndef background_H
#define background_H


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

#define NUM_FRAMES 6
#define FRAME_DUR 80
#define NUM_ANIM 2

using namespace glm;

class background :
	public Engine::Game
{
public:
	background();
	~background();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	void buildBackground();
	void drawBackground();

private:
	//background
	GLuint VAOback, VBOback, EBOback, Textureback,program2;
	float frame_widthBack = 0, frame_heightBack = 0;

};
#endif

