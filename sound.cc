/* Tower Toppler - Nebulus
 * Copyright (C) 2000-2003  Andreas Röver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "sound.h"

#include "decl.h"
#include "configuration.h"

#include <SDL.h>

#ifdef SDL_MIXER
#include <SDL_mixer.h>
#endif

#include <stdlib.h>

#ifdef SDL_MIXER
static Mix_Chunk *sounds[19];
static Mix_Music *title, *tgame;
#endif
static int waterchannel;
static int boinkmax = 0;
static int splashmax = 0;

static long play;

static bool nosoundinit;

#ifdef SDL_MIXER
static Mix_Chunk *LoadWAV(char *name) {
  FILE *f = open_data_file(name);

  if (f)
    return Mix_LoadWAV_RW(SDL_RWFromFP(f, 1), 1);
  else
    return NULL;
}
#endif

void snd_init(void) {
  if (config.nosound()) return;

#ifdef SDL_MIXER
  if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
    printf("Couldn't init the sound system, muting.\n");
    nosoundinit = true;
    return;
  }

  nosoundinit = false;

  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
    printf("Could not open audio, muting.\n");
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    nosoundinit = true;
    return;
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
  sounds[18] = LoadWAV("sonar.wav");

  title = Mix_LoadMUS("title.xm");
  tgame = Mix_LoadMUS("tower.xm");
#endif
}

void snd_done(void) {
  if (config.nosound() || nosoundinit) return;

#ifdef SDL_MIXER
  while (Mix_Playing(-1)) dcl_wait();

  for (int t = 0; t < 18; t++)
    if (sounds[t])
      Mix_FreeChunk(sounds[t]);

  Mix_FreeMusic(title);

  Mix_CloseAudio();

  SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
}

void snd_tap(void) {       play |= 0x1; }
void snd_boink(int vol) {  play |= 0x2; if (vol > boinkmax) boinkmax = vol; }
void snd_hit(void) {       play |= 0x4; }
void snd_cross(void) {     play |= 0x8; }
void snd_tick(void) {      play |= 0x10; }
void snd_drown(void) {     play |= 0x20; }
void snd_splash(int vol) { play |= 0x40; if (vol > splashmax) splashmax = vol; }
void snd_shoot(void) {     play |= 0x80; }                 
void snd_alarm(void) {     play |= 0x100; }                
void snd_score(void) {     play |= 0x200; }                
void snd_crumble(void) {   play |= 0x400; }                
void snd_fanfare(void) {   play |= 0x800; }                
void snd_doortap(void) {   play |= 0x1000; }               
void snd_sub_raise(void) { play |= 0x2000; }               
void snd_sub_down(void) {  play |= 0x4000; }               
void snd_start(void) {     play |= 0x8000; }               
void snd_timeout(void) {   play |= 0x10000; }              
void snd_fall(void) {      play |= 0x20000; }              
void snd_sonar(void) {     play |= 0x40000; }

void snd_play(void) {

  if (config.nosound() || nosoundinit) return;

#ifdef SDL_MIXER
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
  if (play & 0x1000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[1], 0), MIX_MAX_VOLUME);
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
  if (play & 0x40000)
    Mix_Volume(Mix_PlayChannel(-1, sounds[18], 0), MIX_MAX_VOLUME / 4);
#endif

  play = 0;
  splashmax = boinkmax = 0;
}

void snd_wateron(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  if (config.use_water())
    waterchannel = Mix_PlayChannel(-1, sounds[0], -1);
#endif
}
void snd_wateroff(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  if (config.use_water())
    Mix_HaltChannel(waterchannel);
#endif
}
void snd_watervolume(int v) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  if (config.use_water())
    Mix_Volume(waterchannel, v);
#endif
}

void snd_playtitle(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  Mix_PlayMusic(title, -1);
#endif
}

void snd_stoptitle(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  Mix_FadeOutMusic(1000);

  while (Mix_FadingMusic() != MIX_NO_FADING) dcl_wait();
#endif
}

void snd_playtgame(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  Mix_PlayMusic(tgame, -1);
#endif
}

void snd_stoptgame(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef SDL_MIXER
  Mix_FadeOutMusic(1000);

  while (Mix_FadingMusic() != MIX_NO_FADING) dcl_wait();
#endif
}

