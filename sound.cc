#include "sound.h"

#include "decl.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>

static Mix_Chunk *sounds[10];
static int waterchannel;
static int boinkmax = 0;
static int splashmax = 0;

static long play;

void snd_init() {
  if (nosound) return;

  SDL_InitSubSystem(SDL_INIT_AUDIO);

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    assert(0, "cant open audio\n");
  sounds[0] = Mix_LoadWAV(DATADIR"/water.wav");
  sounds[1] = Mix_LoadWAV(DATADIR"/tap.wav");
  sounds[2] = Mix_LoadWAV(DATADIR"/hit.wav");
  sounds[3] = Mix_LoadWAV(DATADIR"/honk.wav");
  sounds[4] = Mix_LoadWAV(DATADIR"/tick.wav");
  sounds[5] = Mix_LoadWAV(DATADIR"/boing.wav");
  sounds[6] = Mix_LoadWAV(DATADIR"/bubbles.wav");
  sounds[7] = Mix_LoadWAV(DATADIR"/splash.wav");
  sounds[8] = Mix_LoadWAV(DATADIR"/swosh.wav");
  assert(sounds[0] != 0, "could not load water\n");
  assert(sounds[1] != 0, "could not load tap\n");
  assert(sounds[2] != 0, "could not load hit\n");
  assert(sounds[3] != 0, "could not load honk\n");
  assert(sounds[4] != 0, "could not load tick\n");
  assert(sounds[5] != 0, "could not load boing\n");
}

void snd_done() {
  if (nosound) return;

  while (Mix_Playing(-1)) dcl_wait();

  Mix_FreeChunk(sounds[0]);
  Mix_FreeChunk(sounds[1]);
  Mix_FreeChunk(sounds[2]);
  Mix_FreeChunk(sounds[3]);
  Mix_FreeChunk(sounds[4]);
  Mix_FreeChunk(sounds[5]);

  Mix_CloseAudio();

  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void snd_tap() {
  play |= 1;
}

void snd_tick() {
  play |= 16;
}
void snd_door() {}
void snd_cross() {
  play |= 8;
}
void snd_shoot() {
  play |= 0x80;
}
void snd_hit() {
  play |= 4;
}
void snd_splash(int vol) {
  play |= 0x40;
  if (vol > splashmax) splashmax = vol;
}
void snd_drown() {
  play |= 0x20;
}
void snd_sub_raise() {}
void snd_sub_down() {}
void snd_boink(int vol) {
  play |= 2;
  if (vol > boinkmax) boinkmax = vol;
}

void snd_play() {
  if (nosound) return;

  if (play & 0x01)
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), MIX_MAX_VOLUME);
  if (play & 0x02)
    Mix_Volume(Mix_PlayChannel(-1, sounds[5], 0), boinkmax);
  if (play & 0x04)
    Mix_Volume(Mix_PlayChannel(-1, sounds[2], 0), MIX_MAX_VOLUME);
  if (play & 0x08)
    Mix_Volume(Mix_PlayChannel(-1, sounds[3], 1), MIX_MAX_VOLUME);
  if (play & 0x10)
    Mix_Volume(Mix_PlayChannel(-1, sounds[4], 0), MIX_MAX_VOLUME);
  if (play & 0x20)
    Mix_Volume(Mix_PlayChannel(-1, sounds[6], 2), MIX_MAX_VOLUME);
  if (play & 0x40)
    Mix_Volume(Mix_PlayChannel(-1, sounds[7], 0), splashmax);
  if (play & 0x80)
    Mix_Volume(Mix_PlayChannel(-1, sounds[8], 0), MIX_MAX_VOLUME);

  play = 0;
  splashmax = boinkmax = 0;
}

void snd_wateron() {
  if (nosound) return;
  waterchannel = Mix_PlayChannel(-1, sounds[0], -1);
}
void snd_wateroff() {
  if (nosound) return;
  Mix_HaltChannel(waterchannel);
}
void snd_watervolume(int v) {
  if (nosound) return;
  Mix_Volume(waterchannel, v);
}

