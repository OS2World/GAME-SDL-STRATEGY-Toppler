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

#include <string.h>
#include "sound.h"

#include "decl.h"
#include "archi.h"
#include "configuration.h"

static bool samplesloaded = false;

void snd_init(void) {
  if (!config.nosound()) {
    ttsounds::instance()->opensound();

    if (!samplesloaded) {

      ttsounds::instance()->addsound("water.wav",    SND_WATER,     128, -1);
      ttsounds::instance()->addsound("tap.wav",      SND_TAP,       MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("boing.wav",    SND_BOINK,     0,    0);
      ttsounds::instance()->addsound("hit.wav",      SND_HIT,       MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("honk.wav",     SND_CROSS,     MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("tick.wav",     SND_TICK,      MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("bubbles.wav",  SND_DROWN,     MIX_MAX_VOLUME, 2);
      ttsounds::instance()->addsound("splash.wav",   SND_SPLASH,    0,    0);
      ttsounds::instance()->addsound("swoosh.wav",   SND_SHOOT,     MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("alarm.wav",    SND_ALARM,     MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("score.wav",    SND_SCORE,     MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("rumble.wav",   SND_CRUMBLE,   MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("fanfare.wav",  SND_FANFARE,   MIX_MAX_VOLUME, 0);
      ttsounds::instance()->addsound("sonar.wav",    SND_SONAR,     MIX_MAX_VOLUME/6, 0);
      ttsounds::instance()->addsound("torpedo.wav",  SND_TORPEDO,   MIX_MAX_VOLUME, 0);
      //TTSound->addsound("subfall.wav",  SND_SUB_DOWN,  MIX_MAX_VOLUME, 0);
      //TTSound->addsound("subraise.wav", SND_SUB_RAISE, MIX_MAX_VOLUME, 0);
      //TTSound->addsound("start.wav",    SND_START,     MIX_MAX_VOLUME, 0);
      //TTSound->addsound("timeout.wav",  SND_TIMEOUT,   MIX_MAX_VOLUME, 0);
      //TTSound->addsound("fall.wav",     SND_FALL,      MIX_MAX_VOLUME, 0);

      samplesloaded = true;
    }
  }
}

void snd_done(void) {
  ttsounds::instance()->closesound();
}

void snd_playtgame(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef HAVE_LIBSDL_MIXER
  Mix_PlayMusic(tgame, -1);
#endif
}

void snd_stoptgame(void) {
  if (config.nosound() || nosoundinit) return;
#ifdef HAVE_LIBSDL_MIXER
  Mix_FadeOutMusic(1000);

void snd_torpedoStop(void) {
  Mix_HaltChannel(torpedochannel);
}

void snd_play(void) {

    if (config.nosound()) return;

    //FIXME: the SDL audio init stuff should be moved out of this class
#ifdef HAVE_LIBSDL_MIXER
    if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
	debugprintf(0,"Couldn't init the sound system, muting.\n");
	nosoundinit = true;
	return;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
	printf("Could not open audio, muting.\n");
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	nosoundinit = true;
	return;
    }
#endif

    nosoundinit = false;
#ifdef DEBUG
    debugprintf(9, "ttsounds::ttsounds\n");
#endif
}

ttsounds::~ttsounds(void)
{
    if (config.nosound() || nosoundinit) return;

#ifdef HAVE_LIBSDL_MIXER
    while (Mix_Playing(-1)) dcl_wait();

    for (int t = 0; t < n_sounds; t++)
      if (sounds[t].sound) 
	Mix_FreeChunk(sounds[t].sound);
#endif

    delete [] sounds;
    
    //Maybe music should be in somewhere else?
    //Or maybe we should extend this class to handle music too?
    //Mix_FreeMusic(title);

    //FIXME: these should be moved out of here
#ifdef HAVE_LIBSDL_MIXER
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
#ifdef DEBUG
    debugprintf(9, "ttsounds::~ttsounds\n");
#endif
}

void ttsounds::addsound(char *fname, int id, int vol, int loops)
{
    struct ttsnddat *tmp;
    bool need_add = true;
    int add_pos = n_sounds;

    if (config.nosound() || nosoundinit) return;

    if (sounds && n_sounds)
      for (int t = 0; t < n_sounds; t++)
	if (!sounds[t].in_use) {
	    need_add = false;
	    add_pos = t;
	    break;
	}

    if (need_add) {
	tmp = new struct ttsnddat [n_sounds + 1];

	if (!tmp) return;

	if (n_sounds) {
	    memcpy(tmp, sounds, sizeof(struct ttsnddat) * n_sounds);
	    delete [] sounds;
	}
	sounds = tmp;
    }

#ifdef HAVE_LIBSDL_MIXER
    sounds[add_pos].sound = LoadWAV(fname);
#endif
    if (sounds[add_pos].sound) {
	sounds[add_pos].in_use = true;
	sounds[add_pos].play = false;
	sounds[add_pos].id_num = id;
	sounds[add_pos].channel = -1;
	sounds[add_pos].volume = vol;
	sounds[add_pos].loops = loops;
#ifdef DEBUG
	debugprintf(8,"ttsounds::addsound(\"%s\", %i, %i) = %i\n", fname, vol, loops, add_pos);
    } else debugprintf(0,"ttsounds::addsound(): No such file as '%s'\n", fname);
#else
    }
#endif

    n_sounds++;

    //return add_pos;
    return;
}

/*
void ttsounds::delsound(int snd)
{
    if (config.nosound() || nosoundinit) return;

    if ((snd >= 0) && (snd < n_sounds) && 
	(sounds[snd].sound || sounds[snd].in_use)) {
	stopsound(snd);
#ifdef HAVE_LIBSDL_MIXER
	if (sounds[snd].sound) 
	  Mix_FreeChunk(sounds[snd].sound);
#endif
	sounds[snd].in_use = false;
	sounds[snd].play = false;
    }
#ifdef DEBUG
    debugprintf(9, "ttsounds::delsound(%i)\n", snd);
#endif
}
*/

void ttsounds::play(void)
{
    if (config.nosound() || nosoundinit) return;
    
    for (int t = 0; t < n_sounds; t++)
      if (sounds[t].in_use && sounds[t].play) {
#ifdef HAVE_LIBSDL_MIXER
	  sounds[t].channel = Mix_PlayChannel(-1, sounds[t].sound, sounds[t].loops);
	  Mix_Volume(sounds[t].channel, sounds[t].volume);
#endif
	  sounds[t].play = false;
      }
#ifdef DEBUG
    debugprintf(9,"ttsounds::play()\n");
#endif
}

void ttsounds::stop(void)
{
    for (int t = 0; t < n_sounds; t++) stopsound(t);
}

void ttsounds::stopsound(int snd)
{
    if (config.nosound() || nosoundinit) return;

    if ((snd >= 0) && (snd < n_sounds)) {
	if (/*sounds[snd].play &&*/ (sounds[snd].channel != -1)) {
#ifdef HAVE_LIBSDL_MIXER
	    Mix_HaltChannel(sounds[snd].channel);
#endif
	    sounds[snd].channel = -1;
	}
	sounds[snd].play = false;
    }
#ifdef DEBUG
    debugprintf(9,"ttsounds::stopsound(%i)\n", snd);
#endif
}

void ttsounds::startsound(int snd)
{
    if (config.nosound() || nosoundinit) return;

    if ((snd >= 0) && (snd < n_sounds)) sounds[snd].play = true;
#ifdef DEBUG
    debugprintf(9,"ttsounds::startsound(%i)\n", snd);
#endif
}

void ttsounds::setsoundvol(int snd, int vol)
{
    if (config.nosound() || nosoundinit) return;

    if ((snd >= 0) && (snd < n_sounds)) {
	if (/*sounds[snd].play &&*/ (sounds[snd].channel != -1)) {
#ifdef HAVE_LIBSDL_MIXER
	    Mix_Volume(sounds[snd].channel, vol);
#endif
	}
	sounds[snd].volume = vol;
    }
#ifdef DEBUG
    debugprintf(9,"ttsounds::setsoundvol(%i, %i)\n", snd, vol);
#endif
}

ttsounds *TTSound;
