#include "Demo.h"
#include "sound.h"
#include "FileScoreHandler.h"
//#include "textProcess.h"
//textProcess mytext;
sound sound1, sound2,soundMenu;
FileScoreHandler fileScore;
Demo::Demo()
{

}
Demo::~Demo()
{

}

void Demo::Init()
{
	//mytext.Init();
	highscore = fileScore.readFromfile("Highscore.txt");
	sound1.InitAudio("Sound/ED6110.ogg", "Sound/true.wav");
	sound2.InitAudio("Sound/ED6105.ogg", "Sound/false.wav");
	soundMenu.InitAudio("", "Sound/menu.wav");
	MapInput();
	score = 0;
	srand(time(NULL));
	InitText();
	fruitInit();
	this->program3 = BuildShader("shader.vert", "shader.frag");
	BuildPlayerSprite();
	buildBackground(&Textureback1, "Image/bgMenu.png");
	buildBackground(&textureback2, "Image/back.jpg");
	buildPointer();
	pointerPosX = GetScreenWidth()/4+100;
	pointerPosY = GetScreenHeight()/2-60;
}

void Demo::Update(float deltaTime)
{
	if (currentMenu == 0) {
		sound2.playBGM();
		if (IsKeyDown("Move Up")) {
			selected--;
			if (selected == 0) selected = 1;
			else {
				pointerPosY -= 60;
				soundMenu.playSFX();
			}
		}
		if (IsKeyDown("Move Down")) {
			selected++;
			if (selected == maxMenu + 1) selected = maxMenu;
			else {
				pointerPosY += 60;
				soundMenu.playSFX();
			}
		}
		if (IsKeyDown("Select")) {
			if (selected == 1) {
				currentMenu = 1;
				score = 0;
				sound1.playSFX();
				sound2.stopBGM();
			}
			if (selected == 2) {
				SDL_Quit();
				exit(0);
			}
		}
	}
	if (currentMenu == 1) { //gameplaymenu
		//GameplayUpdate
		sound1.playBGM();
		UpdatePlayerSpriteAnim(deltaTime);
		ControlPlayerSprite(deltaTime);
		fruitUpdate(deltaTime);
		//GameplayUpdate
		if (IsKeyDown("Quit")) {
			currentMenu = 9;
			SDL_Delay(100);
		}
	}
	else if (currentMenu == 2) {
		PostGameUpdate();
	}
	else if (currentMenu == 9) {
		if (IsKeyDown("Quit")) {
			currentMenu = 1;
			SDL_Delay(100);
		}
		if (IsKeyDown("Start")) {
			SDL_Quit();
			exit(0);
		}
	}
}

void Demo::Render()
{
	//Setting Viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (currentMenu == 0) { //menu
		//Menu Render
		drawBackground(&Textureback1);
		drawPointer();
		RenderText("By EternalDestiny", GetScreenWidth() / 4+200, GetScreenHeight() / 2.5, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Start Game", GetScreenWidth() / 4 +200, GetScreenHeight() / 2, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Exit Game", GetScreenWidth() / 4 +200, GetScreenHeight() / 2 + 60, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	}
	else	if (currentMenu == 1) { //inGame
		//Gameplay Render
		drawBackground(&textureback2);
		RenderText("Fruit Catcher", 10, 10, 1.0f, vec3(244.0f / 255.0f, 12.0f / 255.0f, 116.0f / 255.0f));
		RenderText("Score " + std::to_string(score), 10, 50, 1.0f, vec3(244.0f / 255.0f, 12.0f / 255.0f, 116.0f / 255.0f));
		RenderText("Health " + std::to_string(health), 10, 100, 1.0f, vec3(244.0f / 255.0f, 12.0f / 255.0f, 116.0f / 255.0f));
		fruitRender();
		DrawPlayerSprite();
	}
	else if (currentMenu == 2) { //postgame score
		PostGameRender();
	}
	else if (currentMenu == 9) { //pause
		drawBackground(&textureback2);
		RenderText("Press Esc or (Y) to Continue", GetScreenWidth() / 4, GetScreenHeight() / 2, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Press X or (X) to Exit", GetScreenWidth() / 4, GetScreenHeight() / 2 + 60, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	}
}

void Demo::UpdatePlayerSpriteAnim(float deltaTime)
{
	// Update animation
	frame_dur += deltaTime;

	if (walk_anim && frame_dur > FRAME_DUR) {
		frame_dur = 0;
		if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;

		// Pass frameIndex to shader
		UseShader(this->program);
		glUniform1i(glGetUniformLocation(this->program, "frameIndex"), frame_idx);
		glUniform1i(glGetUniformLocation(this->program, "animIndex"), 1);
	}
	else if (!walk_anim && frame_dur > FRAME_DUR)
	{
		frame_dur = 0;
		if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;

		// Pass frameIndex to shader
		UseShader(this->program);
		glUniform1i(glGetUniformLocation(this->program, "frameIndex"), frame_idx);
		glUniform1i(glGetUniformLocation(this->program, "animIndex"), 1); //ganti ke 0 untuk aktifkan idle
	}
}

void Demo::ControlPlayerSprite(float deltaTime)
{
	walk_anim = false;
	oldxpos = xpos;

	if (IsKeyDown("Move Right")) {
		xpos += deltaTime * xVelocity;
		flip = 0;
		walk_anim = true;
	}

	if (IsKeyDown("Move Left")) {
		xpos -= deltaTime * xVelocity;
		flip = 1;
		walk_anim = true;
	}
	if (xpos < 0 || xpos > GetScreenWidth() - frame_width) {
		xpos = oldxpos;
	}

	// check collision between bart and crate
	//if (IsCollided(xpos, ypos, frame_width, frame_height, FruitPosX[1], FruitPosY[1], frame_width2, frame_height2)) {
	//	//xpos = oldxpos;
	//	exist[1] = false;
	//}
}

void Demo::DrawPlayerSprite() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "ourTexture"), 0);

	// set flip
	glUniform1i(glGetUniformLocation(this->program, "flip"), flip);
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xpos, ypos, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width, frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void Demo::BuildPlayerSprite()
{
	this->program = BuildShader("playerSprite.vert", "playerSprite.frag");

	// Pass n to shader
	UseShader(this->program);
	glUniform1f(glGetUniformLocation(this->program, "n"), 1.0f / NUM_FRAMES);
	glUniform1f(glGetUniformLocation(this->program, "m"), 1.0f / NUM_ANIM);

	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

										   // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("Image/char1.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width = ((float)width) / NUM_FRAMES;
	frame_height = height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));

	// set sprite position, gravity, velocity
	xpos = (GetScreenWidth() - frame_width) / 2;
	yposGround = GetScreenHeight() - frame_height;
	ypos = yposGround;
	gravity = 0.05f;
	xVelocity = 0.4f * 2;

}

void Demo::BuildFruit(float posX, float posY, int idx, GLuint *VAO, GLuint *tex, char* tName) {
	this->program2 = BuildShader("normal.vert", "normal.frag");
	UseShader(this->program2);

	// Load and create a texture 
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

										// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image(tName, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width2 = width;
	frame_height2 = height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection));

	// set sprite position, gravity, velocity
	FruitPosX[idx] = posX;
	FruitPosY[idx] = posY;
}
void Demo::DrawFruit(int idx, GLuint *VAO, GLuint *tex) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *tex);
	// Activate shader
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 1);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(FruitPosX[idx], FruitPosY[idx], 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width2, frame_height2, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(*VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);

}
void Demo::generateFruit(float deltatime) {
	frameDurFruit += deltatime;
	if (frameDurFruit > timebetweenFruit) {
		frameDurFruit = 0;
		for (int i = 0; i < maxFruitCount; i++) {
			if (!exist[i]) {
				exist[i] = true;
				int rdm = rand() % (maxFruitCount / divider);
				FruitPosX[i] = FruitPosFall[rdm];
				FruitPosY[i] = 0;
				break;
			}
		}
	}

}
void Demo::DestroyFall() {
	for (int i = 0; i < maxFruitCount; i++) {
		if (exist[i]) {
			if (FruitPosY[i] > GetScreenHeight()) {
				exist[i] = false;
			}
		}
	}
}
void Demo::BuildAll() {
	for (int i = 0; i < maxFruitCount; i++) {
		if (i == 0 || i == 10) {
			if (i == 0)
				BuildFruit(0, 0, i, &VAObomb1, &texbomb1, "Image/apelMerahUlat.png");
			else
				BuildFruit(0, 0, i, &VAObomb2, &texbomb2, "Image/apelHijauUlat.png");
		}
		else {
			if (i % 2 == 0)
				BuildFruit(0, 0, i, &VAOFruit1, &texFruit1, "Image/apelHijau.png");
			if (i % 2 == 1)
				BuildFruit(0, 0, i, &VAOFruit2, &texFruit2, "Image/apelMerah.png");
		}
		exist[i] = false;
	}
}
void Demo::CollisionCheck() {
	for (int i = 0; i < maxFruitCount; i++) {
		if (exist[i]) {
			if (IsCollided(xpos, ypos, frame_width, frame_height, FruitPosX[i], FruitPosY[i], frame_width2, frame_height2)) {
				exist[i] = false;
				if (i == 0 || i == 10) {
					//tesDeath
					sound2.playSFX();
					fallspeed -= 4.0;
					timebetweenFruit += 200;
					if (fallspeed < 2) {
						fallspeed = 2;
					}
					if (timebetweenFruit > 1200) {
						timebetweenFruit = 1200;
					}
					health--;
					//printf("Jumlah Health: %d\n", health);
					if (health == 0)
					{
						restartGame();
					}
				}
				else {
					sound1.playSFX();
					score += 10;
					//printf("Score: %d\n", score);
					if (score % 50 == 0) {
						fallspeed += 0.5;
						timebetweenFruit -= 50;
						//printf("FallSpeed %f\n", fallspeed);
					}
				}

			}
		}
	}
}
void Demo::restartGame() {
	xpos = (GetScreenWidth() - frame_width) / 2;
	ypos = GetScreenHeight() - frame_height;
	//score = 0;
	health = 3;
	fallspeed = 2;
	timebetweenFruit = 1200;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BuildAll();
	currentMenu = 2;
	//mysound.stopBGM();
}
bool Demo::IsCollided(float x1, float y1, float width1, float height1,
	float x2, float y2, float width2, float height2) {
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2 && y2<y1);
}
void Demo::setFallPos() {
	int onepos = GetScreenWidth() / (maxFruitCount / divider);
	for (int i = 0; i < maxFruitCount / divider; i++) {
		FruitPosFall[i] = i * onepos;
	}
}
void Demo::fruitInit() {
	divider = 2;
	maxFruitCount = 20;
	fruitCount = 0;
	FruitPosY.resize(maxFruitCount);
	FruitPosX.resize(maxFruitCount);
	FruitPosFall.resize(maxFruitCount / divider);
	BuildAll();
	setFallPos();
}
void Demo::fruitUpdate(float deltaTime) {
	generateFruit(deltaTime);
	DestroyFall();
	CollisionCheck();
	//FallSpeed
	for (int i = 0; i < FruitPosX.size(); i++) {
		FruitPosY[i] += fallspeed;
	}
}
void Demo::fruitRender() {
	for (int i = 0; i < maxFruitCount; i++) {
		if (exist[i]) {
			if (i == 0 || i == 10) {
				if (i == 0)
					DrawFruit(i, &VAObomb1, &texbomb1);
				else
					DrawFruit(i, &VAObomb2, &texbomb2);
			}
			else {
				if (i % 2 == 0)
					DrawFruit(i, &VAOFruit1, &texFruit1);
				else
					DrawFruit(i, &VAOFruit2, &texFruit2);
			}
		}
	}
}
void Demo::InitText() {
	// Init Freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		//Err("ERROR::FREETYPE: Could not init FreeType Library");
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		exit(1);
	}
	FT_Face face;
	if (FT_New_Face(ft, FONTNAME, 0, &face)) {
		//Err("ERROR::FREETYPE: Failed to load font");
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		exit(2);
	}

	FT_Set_Pixel_Sizes(face, 0, FONTSIZE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		//GLuint texturete;
		glGenTextures(1, &Texturetext);
		glBindTexture(GL_TEXTURE_2D, Texturetext);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			Texturetext,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAOtext);
	glGenBuffers(1, &VBOtext);
	glBindVertexArray(VAOtext);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtext);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Demo::RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color)
{
	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "projection"), 1, GL_FALSE, value_ptr(projection));
	// Activate corresponding render state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UseShader(this->program3);

	glUniform3f(glGetUniformLocation(this->program3, "ourColor"), color.x, color.y, color.z);
	glUniform1i(glGetUniformLocation(this->program3, "text"), 1);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(this->program3, "ourTexture"), 2);
	mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "model"), 1, GL_FALSE, value_ptr(model));
	glBindVertexArray(VAOtext);
	// Iterate through all characters
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character

		GLfloat vertices[] = {
			// Positions   // Texture Coords
			xpos + w,	ypos + h,	1.0f, 1.0f, // Bottom Right
			xpos + w,	ypos,		1.0f, 0.0f, // Top Right
			xpos,		ypos,		0.0f, 0.0f, // Top Left
			xpos,		ypos + h,	0.0f, 1.0f  // Bottom Left 
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOtext);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_QUADS, 0, 4);
		// Now advance cursors for next glyph (note that advance is number of 1 / 64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}
void Demo::buildPointer() {
	this->program2 = BuildShader("normal.vert", "normal.frag");
	UseShader(this->program2);

	// Load and create a texture 
	glGenTextures(1, &texturePointer);
	glBindTexture(GL_TEXTURE_2D, texturePointer);
	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("Image/selector2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
									 /*frame_widthBack = width;
									 frame_heightBack = height;*/
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOback);
	glGenBuffers(1, &VBOback);
	glGenBuffers(1, &EBOback);

	glBindVertexArray(VAOback);

	glBindBuffer(GL_ARRAY_BUFFER, VBOback);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOback);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection));
}
void Demo::drawPointer() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texturePointer);
	// Activate shader
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 4);

	mat4 model;
	// Translate 
	model = translate(model, vec3(pointerPosX, pointerPosY, 0));
	// Scale sprite 
	model = scale(model, vec3(GetScreenWidth()/3, GetScreenHeight()/6, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOback);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}
void Demo::buildBackground(GLuint *texture, char* file) {
	this->program2 = BuildShader("normal.vert", "normal.frag");
	UseShader(this->program2);

	// Load and create a texture 
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
									 /*frame_widthBack = width;
									 frame_heightBack = height;*/
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOback);
	glGenBuffers(1, &VBOback);
	glGenBuffers(1, &EBOback);

	glBindVertexArray(VAOback);

	glBindBuffer(GL_ARRAY_BUFFER, VBOback);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOback);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection));
}
void Demo::drawBackground(GLuint *texture) {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// Activate shader
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 3);

	mat4 model;
	// Translate sprite along x-axis
	// Scale sprite 
	model = scale(model, vec3(GetScreenWidth(), GetScreenHeight(), 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOback);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}
void Demo::PostGameRender() {
	drawBackground(&textureback2);
	if (score > highscore) {
		RenderText("Congratulations! New HighScore!", GetScreenWidth() / 5, GetScreenHeight() / 4, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Your Score :" + std::to_string(score), GetScreenWidth() / 5, GetScreenHeight() / 3, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Previous Highscore :" + std::to_string(highscore), GetScreenWidth() / 2, GetScreenHeight() / 3, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	}
	else {
		RenderText("Your Score :" + std::to_string(score), GetScreenWidth() / 4, GetScreenHeight() / 4, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
		RenderText("Highscore :" + std::to_string(highscore), GetScreenWidth() / 4, GetScreenHeight() / 3, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	}
	RenderText("Press X or (X) to Play Again", GetScreenWidth() / 4, GetScreenHeight() / 2, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));
	RenderText("Press Esc or (Y) to Main Menu", GetScreenWidth() / 4, GetScreenHeight() / 1.5f, 1.0f, vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));

}
void Demo::PostGameUpdate() {
	if (IsKeyDown("Start")) {
		currentMenu = 1;
		if (score > highscore) {
			highscore = score;
			fileScore.writetoFile("Highscore.txt", std::to_string(highscore));
		}
		score = 0;
	}
	if (IsKeyDown("Quit")) {
		currentMenu = 0;
		if (score > highscore) {
			highscore = score;
			fileScore.writetoFile("Highscore.txt", std::to_string(highscore));
		}
		score = 0;
		sound1.stopBGM();
		SDL_Delay(400);
	}
}
void Demo::MapInput() {
	// Add input mapping
	// to offer input change flexibility you can save the input mapping configuration in a configuration file (non-hard code) !
	InputMapping("Move Right", SDLK_RIGHT);
	InputMapping("Move Left", SDLK_LEFT);
	InputMapping("Move Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	InputMapping("Move Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	InputMapping("Quit", SDLK_ESCAPE);
	InputMapping("Quit", SDL_CONTROLLER_BUTTON_Y);
	InputMapping("Quit", SDL_CONTROLLER_BUTTON_START);
	InputMapping("Start", SDLK_x);
	InputMapping("Start", SDL_CONTROLLER_BUTTON_X);
	InputMapping("Select", SDLK_SPACE);
	InputMapping("Select", SDLK_RETURN);
	//InputMapping("Select", SDL_CONTROLLER_BUTTON_X);
	InputMapping("Select", SDL_CONTROLLER_BUTTON_A);
	//InputMapping("Select", SDL_CONTROLLER_BUTTON_B);
	InputMapping("Move Down", SDLK_DOWN);
	InputMapping("Move Down", SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	InputMapping("Move Up", SDLK_UP);
	InputMapping("Move Up", SDL_CONTROLLER_BUTTON_DPAD_UP);
}
int main(int argc, char** argv) {

	Engine::Game &game = Demo();
	game.Start("Fruit Catcher", 1366, 768, false, WindowFlag::WINDOWED, 60, 1);

	return 0;
}
