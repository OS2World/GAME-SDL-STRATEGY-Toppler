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

#ifndef SOUND_H
#define SOUND_H

#include "config.h"

#ifdef HAVE_LIBSDL_MIXER
#include <SDL_mixer.h>
#endif

/* this module handles all the soundoutput necessary for the game */

struct ttsnddat {
    bool in_use; //is this datablock in use (sndfile is loaded)
    bool play;   //is this block goind to get played next time?
    int id_num;  //unique ID # of this sound
    int channel; //sound channel
    int volume;  //sound volume
    int loops;   //how many times to loop this sound?
#ifdef HAVE_LIBSDL_MIXER
    Mix_Chunk *sound; //sound data
#endif
};

class ttsounds {
   public:
      ttsounds(void);
      ~ttsounds(void);

      void addsound(char *fname, int id, int vol, int loops);
      //void delsound(int snd);

      void play(void); //play all active sounds
      void stop(void); //stop all sounds

      void stopsound(int snd); //stop the sound from playing
      void startsound(int snd); //the sound will play in the next update
      void setsoundvol(int snd, int vol); //set sound volume
   private:
      bool nosoundinit;
      int n_sounds; // # of sounds allocated
      struct ttsnddat *sounds;
};

extern ttsounds *TTSound;



#define SND_WATER 0
#define SND_TAP 1
#define SND_DOORTAP 1
#define SND_BOINK 2
#define SND_HIT 3
#define SND_CROSS 4
#define SND_TICK 5
#define SND_DROWN 6
#define SND_SPLASH 7
#define SND_SHOOT 8
#define SND_ALARM 9
#define SND_SCORE 10
#define SND_CRUMBLE 11
#define SND_FANFARE 12
#define SND_SONAR 13
#define SND_TORPEDO 14
//#define SND_SUB_RAISE 13
//#define SND_SUB_DOWN 14
//#define SND_START 13
//#define SND_TIMEOUT 16
//#define SND_FALL 17

void snd_init(void);
void snd_done(void);

#endif

