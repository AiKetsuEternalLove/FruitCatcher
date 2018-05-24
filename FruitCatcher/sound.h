#pragma once
#include <SDL/SDL_mixer.h>
#include <stdio.h>
class sound
{
public:
	sound();
	~sound();
	void InitAudio(char *bgm, char *sfx);
	void playBGM();
	void stopBGM();
	void playSFX();
private:
	Mix_Chunk *effect = NULL;
	Mix_Music *music = NULL;
	int sfx_channel = -1;
};