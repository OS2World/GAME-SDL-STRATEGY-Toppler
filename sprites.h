/* Tower Toppler - Nebulus
 * Copyright (C) 2000-2002  Andreas R�ver
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

#ifndef SPRITES_H
#define SPRITES_H

#include <SDL.h>

/* coordinates a collection of sprites */

/* initializes with a maximum number of possible prites */
void spr_init(Uint16 maxsp);

/* frees all sprites */
void spr_done(void);

/* enters one sprite into the collection and returns a unique index */
long spr_savesprite(SDL_Surface *s);

/* returns the sprite data for the given index */
SDL_Surface *spr_spritedata(Uint16 nr);

#endif
