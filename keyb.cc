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

static Uint16 keydown, keytyped;
static char chartyped;
static SDLKey sdlkeytyped;
static long numkeydown;
static bool received_kill;
static Uint16 mouse_x, mouse_y;
static bool mouse_moved;
static Uint16 mouse_button;

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
   {pause_key, SDLK_p},
   {mousebttn1, SDLK_SPACE},
   {mousebttn4, SDLK_UP},
   {mousebttn5, SDLK_DOWN},

   {up_key, SDLK_UP},
   {down_key, SDLK_DOWN},
   {left_key, SDLK_LEFT},
   {right_key, SDLK_RIGHT},
   {fire_key, SDLK_SPACE},
   {break_key, SDLK_ESCAPE},
   {pause_key, SDLK_p},
};

void key_redefine(Uint16 code, SDLKey key) {
    int i;
    
    for (i = SIZE(ttkeyconv) - 1; i >= 0; i--)
	if (ttkeyconv[i].outval == code) {
	    ttkeyconv[i].key = key;
	    break;
	}
}

void key_init(void) {
  SDL_EnableKeyRepeat(0, 0);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

  numkeydown = keydown = keytyped = chartyped = 0;
  sdlkeytyped = SDLK_UNKNOWN;
  received_kill = false;
  mouse_button = mouse_x = mouse_y = 0;
  mouse_moved = false;
}

static void handleEvents(void) {
  SDL_Event e;
  Uint16 key = 0;
  int tmpk;

  while (SDL_PollEvent(&e)) {
    mouse_moved = false;
    mouse_button = 0;
    if (e.type == SDL_MOUSEMOTION) {
	mouse_x = e.motion.x;
	mouse_y = e.motion.y;
	mouse_moved = true;
    } else
    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
	mouse_x = e.button.x;
	mouse_y = e.button.y;
	mouse_button = e.button.button;
    } else
    if (e.type == SDL_QUIT) {
	keydown |= quit_action;
	keytyped |= quit_action;
	received_kill = true;
	fprintf(stderr, "Wheee!!\n");
    } else
    if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP)) {
      if (e.key.state == SDL_RELEASED) {
	numkeydown--;
        if ((e.key.keysym.sym >= SDLK_a) && (e.key.keysym.sym <= SDLK_z)) {
          if (e.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
            chartyped = e.key.keysym.sym - SDLK_a + 'A';
          else
            chartyped = e.key.keysym.sym - SDLK_a + 'a';
        }
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
	
	sdlkeytyped = e.key.keysym.sym;
	numkeydown++;
      } else {
        keydown &= ~key;
	sdlkeytyped = SDLK_UNKNOWN;
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

bool key_keypressed(Uint16 key) {
  handleEvents();
  if (((key | quit_action)) && received_kill) return true;
    return (keytyped & key) != 0;
}

SDLKey key_sdlkey(void) {
    handleEvents();
    SDLKey tmp = sdlkeytyped;
    sdlkeytyped = SDLK_UNKNOWN;
    keytyped = 0;
    chartyped = 0;
    return tmp;
}

SDLKey key_conv2sdlkey(Uint16 k, bool game) {
    register int i;

    if (game) {
	for (i = SIZE(ttkeyconv) - 1; i >= 0; i--)
	  if (ttkeyconv[i].outval == k)
	    return ttkeyconv[i].key;
    } else {
	for (i = 0; i < SIZE(ttkeyconv); i++)
	  if (ttkeyconv[i].outval == k)
	    return ttkeyconv[i].key;
    }

    return SDLK_UNKNOWN;
}

Uint16 key_sdlkey2conv(SDLKey k, bool game) {
    register int i;
    
    if (k != SDLK_UNKNOWN) {
	if (game) {
	    for (i = SIZE(ttkeyconv) - 1; i >= 0; i--)
	      if (ttkeyconv[i].key == k)
		return ttkeyconv[i].outval;
	} else {
	    for (i = 0; i < SIZE(ttkeyconv); i++)
	      if (ttkeyconv[i].key == k)
		return ttkeyconv[i].outval;
	}
    }

    return no_key;
}

Uint16 key_readkey(void) {
  handleEvents();

  Uint16 i = keytyped;

  keytyped = 0;
  chartyped = 0;
  sdlkeytyped = SDLK_UNKNOWN;

  return i;
}

char key_chartyped(void) {
  handleEvents();
  int erg = chartyped;
  chartyped = 0;
  return erg;
}

void key_wait_for_none(keyb_wait_proc bg) {
    do {
	handleEvents();
	if (bg) (*bg)();
    } while (numkeydown && !received_kill);
    keytyped = 0;
    chartyped = 0;
    sdlkeytyped = SDLK_UNKNOWN;
}

bool key_mouse(Uint16 *x, Uint16 *y, Uint16 *bttn) {
    bool tmp = mouse_moved;
    handleEvents();
    switch (mouse_button) {
	default: *bttn = 0; break;
	case 1: *bttn = mousebttn1; break;
	case 2: *bttn = mousebttn2; break;
	case 3: *bttn = mousebttn3; break;
	case 4: *bttn = mousebttn4; break;
	case 5: *bttn = mousebttn5; break;
    }
    mouse_moved = false;
    mouse_button = 0;
    if (tmp) {
	*x = mouse_x;
	*y = mouse_y;
    } else {
	*x = *y = 0;
    }
    return tmp;
}
