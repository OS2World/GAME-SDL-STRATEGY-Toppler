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

#include "keyb.h"
#include "decl.h"

#include <SDL.h>

static ttkey keydown, keytyped;
static char chartyped;
static SDLKey sdlkeytyped;
static Uint16 mouse_x, mouse_y;
static bool mouse_moved;
static Uint16 mouse_button;

class quit_action_class {};

bool tt_has_focus;

struct _ttkeyconv {
   ttkey outval;
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
/*   {mousebttn1, SDLK_SPACE},
   {mousebttn4, SDLK_UP},
   {mousebttn5, SDLK_DOWN},*/

   {up_key, SDLK_UP},
   {down_key, SDLK_DOWN},
   {left_key, SDLK_LEFT},
   {right_key, SDLK_RIGHT},
   {fire_key, SDLK_SPACE},
   {break_key, SDLK_ESCAPE},
   {pause_key, SDLK_p},
};

void key_redefine(ttkey code, SDLKey key) {
  int i;

  for (i = SIZE(ttkeyconv) - 1; i >= 0; i--)
    if (ttkeyconv[i].outval == code) {
      ttkeyconv[i].key = key;
      break;
    }
}

void key_init(void) {
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

  SDL_EnableUNICODE(1);

  chartyped = 0;
  keytyped = keydown = no_key;
  sdlkeytyped = SDLK_UNKNOWN;
  mouse_button = mouse_x = mouse_y = 0;
  mouse_moved = false;
}

static void handleEvents(void) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {

    switch (e.type) {
    case SDL_ACTIVEEVENT:
      if ((e.active.state & SDL_APPINPUTFOCUS) || (e.active.state & SDL_APPACTIVE))
        tt_has_focus = (e.active.gain == 1);
      break;
    case SDL_MOUSEMOTION:
      mouse_x = e.motion.x;
      mouse_y = e.motion.y;
      mouse_moved = true;
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      mouse_x = e.button.x;
      mouse_y = e.button.y;
      mouse_button = e.button.button;
      break;
    case SDL_QUIT:
      fprintf(stderr, "Wheee!!\n");
      throw new quit_action_class;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:

      ttkey key = no_key;
      for (int tmpk = 0; tmpk < SIZE(ttkeyconv); tmpk++)
        if (ttkeyconv[tmpk].key == e.key.keysym.sym) {
          key = ttkeyconv[tmpk].outval;
          break;
        }

      if (e.key.state == SDL_PRESSED) {

        if ((e.key.keysym.unicode & 0xff80) == 0) {
          chartyped = e.key.keysym.unicode & 0x7f;
        } else
          chartyped = 0;

        sdlkeytyped = e.key.keysym.sym;

        keydown = (ttkey)(keydown | key);
        keytyped = (ttkey)(keytyped | key);

      } else {
        keydown = (ttkey)(keydown & ~key);
      }
      break;
    }
  }
}

void key_done(void) {
}

Uint16 key_keystat(void) {
  handleEvents();
  return keydown;
}

bool key_keypressed(ttkey key) {
  handleEvents();
  return (keytyped & key) != 0;
}

SDLKey key_sdlkey(void) {
  handleEvents();
  SDLKey tmp = sdlkeytyped;
  sdlkeytyped = SDLK_UNKNOWN;
  keytyped = no_key;
  chartyped = 0;
  return tmp;
}

void key_keydatas(SDLKey &sdlkey, ttkey &tkey, char &ch) {
  handleEvents();
  sdlkey = sdlkeytyped;
  tkey = keytyped;
  ch = chartyped;
  sdlkeytyped = SDLK_UNKNOWN;
  keytyped = no_key;
  chartyped = 0;
}


SDLKey key_conv2sdlkey(ttkey k, bool game) {
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

ttkey key_sdlkey2conv(SDLKey k, bool game) {
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

ttkey key_readkey(void) {
  handleEvents();

  ttkey i = keytyped;

  keytyped = no_key;
  chartyped = 0;
  sdlkeytyped = SDLK_UNKNOWN;

  return i;
}

void wait_for_focus(void) {

  while (!tt_has_focus) {
    SDL_Delay(200);
    handleEvents();
  }

  keytyped = no_key;
  chartyped = 0;
  sdlkeytyped = SDLK_UNKNOWN;
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
  } while (keydown);

  keytyped = no_key;
  chartyped = 0;
  sdlkeytyped = SDLK_UNKNOWN;
}

bool key_mouse(Uint16 *x, Uint16 *y, ttkey *bttn) {
  bool tmp = mouse_moved;
  handleEvents();
  switch (mouse_button) {
  default: *bttn = no_key; break;
/*  case 1: *bttn = mousebttn1; break;
  case 2: *bttn = mousebttn2; break;
  case 3: *bttn = mousebttn3; break;
  case 4: *bttn = mousebttn4; break;
  case 5: *bttn = mousebttn5; break;*/
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
