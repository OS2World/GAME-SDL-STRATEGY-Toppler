#include "sound.h"

#include "decl.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>

static Mix_Chunk *sounds[18];
static Mix_Music *music;
static int waterchannel;
static int boinkmax = 0;
static int splashmax = 0;

static long play;

static Mix_Chunk *LoadWAV(char *name) {
  FILE *f = open_data_file("water.wav", "rb");

  if (f)
    return Mix_LoadWAV_RW(SDL_RWFromFP(f, 1), 1);
  else
    return NULL;
}

void snd_init() {
  if (nosound) return;

  SDL_InitSubSystem(SDL_INIT_AUDIO);

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    printf("could not open audio, muting\n");
    nosound = true;
  }

  sounds[0] = LoadWAV("water.wav");
  sounds[1] = LoadWAV("tap.wav");
  sounds[2] = LoadWAV("hit.wav");
  sounds[3] = LoadWAV("honk.wav");
  sounds[4] = LoadWAV("tick.wav");
  sounds[5] = LoadWAV("boing.wav");
  sounds[6] = LoadWAV("bubbles.wav");
  sounds[7] = LoadWAV("splash.wav");
  sounds[8] = LoadWAV("swoosh.wav");
  sounds[9] = LoadWAV("alarm.wav");
  sounds[10] = LoadWAV("score.wav");
  sounds[11] = LoadWAV("rumble.wav");
  sounds[12] = LoadWAV("fanfare.wav");
  sounds[13] = LoadWAV("fall.wav");

  sounds[14] = LoadWAV("start.wav");
  sounds[15] = LoadWAV("subfall.wav");
  sounds[16] = LoadWAV("subraise.wav");
  sounds[17] = LoadWAV("timeout.wav");

  music = Mix_LoadMUS("title.ogg");
}

void snd_done() {
  if (nosound) return;

  while (Mix_Playing(-1)) dcl_wait();

  for (int t = 0; t < 18; t++)
    if (sounds[t])
      Mix_FreeChunk(sounds[t]);

  Mix_FreeMusic(music);

  Mix_CloseAudio();

  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void snd_tap() {           play |= 0x1; }
void snd_boink(int vol) {  play |= 0x2; if (vol > boinkmax) boinkmax = vol; }
void snd_hit() {           play |= 0x4; }
void snd_cross() {         play |= 0x8; }
void snd_tick() {          play |= 0x10; }
void snd_drown() {         play |= 0x20; }
void snd_splash(int vol) { play |= 0x40; if (vol > splashmax) splashmax = vol; }
void snd_shoot() {         play |= 0x80; }
void snd_alarm() {         play |= 0x100; }
void snd_score() {         play |= 0x200; }
void snd_crumble() {       play |= 0x400; }
void snd_fanfare() {       play |= 0x800; }
void snd_doortap() {       play |= 0x1000; }
void snd_sub_raise() {     play |= 0x2000; }
void snd_sub_down() {      play |= 0x4000; }
void snd_start() {         play |= 0x8000; }
void snd_timeout() {       play |= 0x10000; }
void snd_fall() {          play |= 0x20000; }

static char doortap = 0;

void snd_play() {

  if (nosound) return;

  if (play & 0x01)
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), MIX_MAX_VOLUME);
  if (play & 0x02)
    Mix_Volume(Mix_PlayChannel(-1, sounds[5], 0), boinkmax);
  if (play & 0x04)
    Mix_Volume(Mix_PlayChannel(-1, sounds[2], 0), MIX_MAX_VOLUME);
  if (play & 0x08)
    Mix_Volume(Mix_PlayChannel(-1, sounds[3], 0), MIX_MAX_VOLUME);
  if (play & 0x10)
    Mix_Volume(Mix_PlayChannel(-1, sounds[4], 0), MIX_MAX_VOLUME);
  if (play & 0x20)
    Mix_Volume(Mix_PlayChannel(-1, sounds[6], 2), MIX_MAX_VOLUME);
  if (play & 0x40)
    Mix_Volume(Mix_PlayChannel(-1, sounds[7], 0), splashmax);
  if (play & 0x80)
    Mix_Volume(Mix_PlayChannel(-1, sounds[8], 0), MIX_MAX_VOLUME);
  if (play & 0x100)
    Mix_Volume(Mix_PlayChannel(-1, sounds[9], 0), MIX_MAX_VOLUME);
  if (play & 0x200)
    Mix_Volume(Mix_PlayChannel(-1, sounds[10], 0), MIX_MAX_VOLUME);
  if (play & 0x400)
    Mix_Volume(Mix_PlayChannel(-1, sounds[11], 0), MIX_MAX_VOLUME);
  if (play & 0x800)
    Mix_Volume(Mix_PlayChannel(-1, sounds[12], 0), MIX_MAX_VOLUME);

  if (doortap) doortap++;

  if (doortap == 2) {
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), 10);
  }
  if (doortap == 3) {
    doortap = 0;
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), 20);
  }
  if (play & 0x1000) {
//    doortap = 1;
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), MIX_MAX_VOLUME);
  }

  if (play & 0x2000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[16], 0), MIX_MAX_VOLUME);
  if (play & 0x4000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[15], 0), MIX_MAX_VOLUME);
  if (play & 0x8000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[14], 0), MIX_MAX_VOLUME);
  if (play & 0x10000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[17], 0), MIX_MAX_VOLUME);
  if (play & 0x20000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[13], 0), MIX_MAX_VOLUME);

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

void snd_playtitle() {
  Mix_PlayMusic(music, -1);
}

void snd_stoptitle() {
  Mix_HaltMusic();
}
