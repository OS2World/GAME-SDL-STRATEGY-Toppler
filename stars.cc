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

#include "stars.h"

#include "sprites.h"
#include "screen.h"

#include "SDL.h"
#include "stdlib.h"

#define starstep 5

static unsigned short starnr;

static struct {
  long x, y;
} stars[21];

void sts_draw(void)
{
  SDL_Surface *p = spr_spritedata((long)starnr);

  for (int t = 0; t <= 20; t++)
    scr_blit(p, stars[t].x, stars[t].y);
}

void sts_init(int sn) {
  for (int t = 0; t <= 20; t++) {
    stars[t].x = rand() / (RAND_MAX / 320) - 16;
    stars[t].y = rand() / (RAND_MAX / 240) - 16;
  }

  starnr = sn;
}

void sts_move(long x, long y)
{
  int t;

  for (t = 0; t <= 20; t++) {
    stars[t].x += starstep * x;
    stars[t].y += y;
  }
  for (t = 0; t <= 20; t++) {
    if (stars[t].x > 320) {
      stars[t].x = rand() / (RAND_MAX / starstep) + 16;
      stars[t].y = rand() / (RAND_MAX / 240);
    } else {
      if (stars[t].x < -16) {
        stars[t].x = 320 - rand() / (RAND_MAX / starstep);
        stars[t].y = rand() / (RAND_MAX / 240);
      }
    }

    if (stars[t].y > 240) {
      stars[t].y = -16;
      stars[t].x = rand() / (RAND_MAX / 336) - 16;
    } else {
      if (stars[t].y < -16) {
        stars[t].y = 240;
        stars[t].x = rand() / (RAND_MAX / 336) - 16;
      }
    }
  }
}

