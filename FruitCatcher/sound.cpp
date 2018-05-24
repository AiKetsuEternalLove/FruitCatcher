
#include "sound.h"


sound::sound()
{
}


sound::~sound()
{
	//if (Mix_Playing(sfx_channel) == 0) {
	//	Mix_FreeChunk(effect);
	//}
	//if (music != NULL) {
	//	Mix_FreeMusic(music);
	//}
	//Mix_CloseAudio();
}
void sound::playBGM() {
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(music, -1);
	}
}
void sound::stopBGM() {
	if (Mix_PlayingMusic() != 0)
	{
		Mix_HaltMusic();
	}
}
void sound::playSFX() {
	sfx_channel = Mix_PlayChannel(-1, effect, 0);
}
void sound::InitAudio(char *bgm, char *sfx) {
	int flags = MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG;
	if (flags != Mix_Init(flags)) {
		//		Err("Unable to initialize mixer: " + string(Mix_GetError()));
		exit(1);
	}

	int audio_rate = 22050; Uint16 audio_format = AUDIO_S16SYS; int audio_channels = 2; int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		//Err("Unable to initialize audio: " + string(Mix_GetError()));
		exit(1);
	}
	if (bgm != "") {
		music = Mix_LoadMUS(bgm);
		if (music == NULL) {
			//Err("Unable to load Music file: " + string(Mix_GetError()));
			//exit(2);
			printf("BGM NOT FOUND \n");
		}
	}
	effect = Mix_LoadWAV(sfx);
	if (effect == NULL) {
		//Err("Unable to load WAV file: " + string(Mix_GetError()));
		//exit(1);
		printf("SFX NOT FOUND \n");
	}
}