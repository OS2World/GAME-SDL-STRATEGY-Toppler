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

#include "sprites.h"

#include "decl.h"

#include <stdlib.h>

static Uint16 maxsprite = 0;
static SDL_Surface **sarray = NULL;

void spr_init(Uint16 maxsp) {
  assert(maxsprite == 0, "sprite init called more than once\n");

  maxsprite = maxsp;

  sarray = new SDL_Surface *[maxsp];

  for (int i = 0; i < maxsprite; i++)
    sarray[i] = NULL;
}

void spr_done(void) {

  delete [] sarray;

  sarray = NULL;
  maxsprite = 0;
}

long spr_savesprite(SDL_Surface *s) {

  Uint16 t = 0;

  while ((sarray[t] != NULL) && (t < maxsprite))
    t++;

  if (t < maxsprite) {
    sarray[t] = s;
    return t;
  } else
    assert(0, "Not enough space for all the sprites!\n");

  return -1;
}

SDL_Surface *spr_spritedata(Uint16 nr) {

  if (nr < maxsprite)
    return sarray[nr];
  else
    return NULL;
}

