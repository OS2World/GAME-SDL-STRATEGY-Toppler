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

#ifndef KEYB_H
#define KEYB_H

#include "decl.h"

#include <SDL_types.h>
#include <SDL_keyboard.h>

#define no_key          0x0000
#define up_key          0x0001
#define down_key        0x0002
#define left_key        0x0004
#define right_key       0x0008
#define fire_key        0x0010
#define break_key       0x0020
#define pause_key       0x0040
#define quit_action     0x0080  /* Not a key, but received on kill */
#define mousebttn1      0x0100
#define mousebttn2      0x0200
#define mousebttn3      0x0400
#define mousebttn4      0x0800
#define mousebttn5      0x1000
#define any_key         0x1FFF

typedef void FDECL((*keyb_wait_proc), (void));

void key_init(void);
void key_done(void);

/* returns bitmask with currently pressed keys */
Uint8 key_keystat(void);

/* true, if key is pressed */
bool key_keypressed(Uint16 key);

/* returns if a key has been pushed and released (typed) but only for the keys in
 the list */
Uint16 key_readkey(void);

/* Returns the last pressed key, in SDLKey format */
SDLKey key_sdlkey(void);

/* Converts sdlkey to internal key representation, or returns no_key
   if SDLKey cannot be converted. */
Uint16 key_sdlkey2conv(SDLKey k, bool game);

/* Converts internal key to sdlkey, or returns SDLK_UNKNOWN. */
SDLKey key_conv2sdlkey(Uint16 k, bool game);

/* returns a types character */
char key_chartyped(void);

/* waits until no key is pressed, calling bg while waiting */
void key_wait_for_none(keyb_wait_proc bg);

/* returns the current mouse coordinate and button pressed. */
bool key_mouse(Uint16 *x, Uint16 *y, Uint16 *bttn);

/* redefine a key, so that it returns code  */
void key_redefine(Uint16 code, SDLKey key);

#endif

