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

#include <SDL_types.h>

#define no_key          0
#define up_key          1
#define down_key        2
#define left_key        4
#define right_key       8
#define fire_key        16
#define break_key       32
#define pause_key       64
#define any_key         255


void key_init(void);
void key_done(void);

/* returns bitmask with currently pressed keys */
Uint8 key_keystat(void);

/* true, if key is pressed */
bool key_keypressed(Uint8 key);

/* returns if a key has been pushed and released (typed) but only for the keys in
 the list */
Uint8 key_readkey(void);

/* returns a types character */
char key_chartyped(void);

/* waits for any keypress */
void key_wait_for_any(void);

/* return name of char */
char *key_name(char c);

#endif

