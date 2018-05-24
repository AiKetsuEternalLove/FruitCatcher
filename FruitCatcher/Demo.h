#ifndef DEMO_H
#define DEMO_H


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

#define FONTSIZE 40
#define FONTNAME "cs_regular.ttf"
#define NUM_BUTTON 3

struct Character {
	GLuint TextureID; // ID handle of the glyph texture
	ivec2 Size; // Size of glyph
	ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance; // Offset to advance to next glyph
};

class Demo :
	public Engine::Game
{
public:
	Demo();
	~Demo();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	void MapInput();
	float frame_width = 0, frame_height = 0, frame_width2 = 0, frame_height2 = 0;
	//fruit
	std::vector<float> FruitPosX, FruitPosY;
	std::vector<float> FruitPosFall;
	int currentMenu = 0;

private:
	float frame_dur = 0, frameDurFruit = 0, oldxpos = 0, xpos = 0, ypos = 0, xpos2 = 0, ypos2 = 0, gravity = 0, xVelocity = 0, yVelocity = 0, yposGround = 0;
	int score, health = 3;
	GLuint VBO, VAO, EBO, texture, program, VBO2, VAO2, EBO2, texture2, program2, program3;
	bool walk_anim = false, onGround = true;
	unsigned int frame_idx = 0, flip = 0;
	void BuildPlayerSprite();
	bool IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
	void DrawPlayerSprite();
	void UpdatePlayerSpriteAnim(float deltaTime);
	void ControlPlayerSprite(float deltaTime);
	void restartGame();

	//fruit
	GLuint VAOFruit1, VAOFruit2, VAOFruit3, texFruit1, texFruit2, texFruit3, VAObomb1, texbomb1,VAObomb2,texbomb2;
	float fallspeed = 2;
	bool exist[20];
	int fruitCount, maxFruitCount, timebetweenFruit = 1200, divider;

	void BuildFruit(float posX, float posY, int idx, GLuint *VAO, GLuint *tex, char* tName);
	void DrawFruit(int idx, GLuint *VAO, GLuint *tex);
	void generateFruit(float deltatime);
	void BuildAll();
	void DestroyFall();
	void CollisionCheck();
	void setFallPos();
	void fruitInit();
	void fruitUpdate(float deltaTime);
	void fruitRender();

	//Text Process
	GLuint VAOtext, VBOtext, EBOtext, Texturetext;
	map<GLchar, Character> Characters;

	void InitText();
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);
	
	//menu selection Pointer
	float pointerPosX, pointerPosY;
	int selected = 1;
	int maxMenu = 2;
	GLuint VAOpointer, VBOpointer, EBOpointer, texturePointer;
	void buildPointer();
	void drawPointer();


	//background Process
	GLuint VAOback, VBOback, EBOback, Textureback1,textureback2;
	float frame_widthBack = 0, frame_heightBack = 0 ;
	void buildBackground(GLuint *texture, char *file);
	void drawBackground(GLuint *texture);
	
	//postgame process
	void PostGameUpdate();
	void PostGameRender();
	int highscore;

};
#endif

