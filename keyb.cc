/* Tower Toppler - Nebulus
 * Copyright (C) 2000-2002  Andreas Röver
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

#include "keyb.h"
#include "decl.h"

#include <SDL.h>

static Uint8 keydown, keytyped;
static char chartyped;

struct _ttkeyconv {
   Uint8 outval;
   SDLKey key;
} static ttkeyconv[] = {
   {up_key, SDLK_UP},
   {down_key, SDLK_DOWN},
   {left_key, SDLK_LEFT},
   {right_key, SDLK_RIGHT},
   {fire_key, SDLK_SPACE},
   {fire_key, SDLK_RETURN},
   {break_key, SDLK_ESCAPE},
   {pause_key, SDLK_p}
};

void key_init(void) {
  SDL_EnableKeyRepeat(0, 0);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

  keydown = keytyped = chartyped = 0;
}

static void handleEvents(void) {
  SDL_Event e;
  Uint8 key = 0;
  int tmpk;

  while (SDL_PollEvent(&e)) {
    if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP)) {

      if (e.key.state == SDL_RELEASED) {
        if ((e.key.keysym.sym >= SDLK_a) && (e.key.keysym.sym <= SDLK_z))
          chartyped = e.key.keysym.sym - SDLK_a + 'A';
        if ((e.key.keysym.sym >= SDLK_0) && (e.key.keysym.sym <= SDLK_9))
          chartyped = e.key.keysym.sym - SDLK_0 + '0';
        if (e.key.keysym.sym == SDLK_SPACE)
          chartyped = ' ';
        if (e.key.keysym.sym == SDLK_RETURN)
          chartyped = 13;
        if (e.key.keysym.sym == SDLK_BACKSPACE)
          chartyped = 8;
        if (e.key.keysym.sym == SDLK_UP)
          chartyped = 1;
        if (e.key.keysym.sym == SDLK_DOWN)
          chartyped = 2;
        if (e.key.keysym.sym == SDLK_LEFT)
          chartyped = 3;
        if (e.key.keysym.sym == SDLK_RIGHT)
          chartyped = 4;
        if (e.key.keysym.sym == SDLK_ESCAPE)
          chartyped = 27;
        if (e.key.keysym.sym == SDLK_GREATER)
          chartyped = '>';
        if (e.key.keysym.sym == SDLK_EXCLAIM)
          chartyped = '!';
        if (e.key.keysym.sym == SDLK_HASH)
          chartyped = '#';
        if (e.key.keysym.sym == SDLK_MINUS)
          chartyped = '-';
        if (e.key.keysym.sym == SDLK_PERIOD)
          chartyped = '.';
        if (e.key.keysym.sym == SDLK_INSERT)
          chartyped = 5;
        if (e.key.keysym.sym == SDLK_DELETE)
          chartyped = 6;
        if (e.key.keysym.sym == SDLK_PAGEUP)
          chartyped = 7;
        if (e.key.keysym.sym == SDLK_PAGEDOWN)
          chartyped = 8;
        if (e.key.keysym.sym == SDLK_HOME)
          chartyped = 9;
      }

      for (tmpk = 0; tmpk < SIZE(ttkeyconv); tmpk++)
	 if (ttkeyconv[tmpk].key == e.key.keysym.sym) {
	    key = ttkeyconv[tmpk].outval;
	    break;
	 }

      if (e.key.state == SDL_PRESSED) {
        keydown |= key;
        keytyped |= key;
      } else {
        keydown &= ~key;
      }
    }
  }
}

void key_done(void) {
}

Uint8 key_keystat(void) {
  handleEvents();
  return keydown;
}

bool key_keypressed(Uint8 key) {
  handleEvents();
  return (keytyped & key);
}

Uint8 key_readkey(void) {
  handleEvents();

  int i = keytyped;

  keytyped = 0;
  chartyped = 0;

  return i;
}

char key_chartyped(void) {
  handleEvents();
  int erg = chartyped;
  chartyped = 0;
  return erg;
}

void key_wait_for_any(void) {
  int c;

  c = key_chartyped();
  do {
     c = key_chartyped();
  } while (!c);
  key_readkey();
}

char *key_name(char c) { 
   static char buf[10];
   switch (c) {
    case ' ': return "space";
    case 1:   return "up";
    case 2:   return "down";
    case 3:   return "left";
    case 4:   return "right";
    case 5:   return "ins";
    case 6:   return "del";
    case 7:   return "pgup";
    case 8:   return "pgdwn";
    case 9:   return "home";
    case 13:  return "enter";
    case 27:  return "esc";
    default:  
      buf[0] = c;
      buf[1] = '\0';
      return buf;
   }
}
