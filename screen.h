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

#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>

/* this modules handles nearly all the output onto the screen */


/* initializes the module, loads the graphics, sets up the display */
void scr_init(void);

void scr_reinit(void);

/* frees graphics */
void scr_done(void);

/* loads a number of sprite, enters it into the sprite collection
 and returns the index of the first sprite */
unsigned short scr_loadsprites(int num, int w, int h, int bits, int colstart, bool sprite);

/* all paint routines paint onto an invisible surface, to show this surface
 call swap */


/* writes some text onto the screen */
void scr_writetext(long x, long y, const char *s);
/* centers the text horizontally */
void scr_writetext_center(long y, const char *s);

/* draws a filles rectangle with color col */
void scr_putbar(int x, int y, int br, int h, unsigned char col = 0);

/* for the submarine game i intend to use a simpel voxel space (maybe)*/
void scr_putvoxel(long xpos, unsigned char (*grnd)[100][256],
                  unsigned char (*ceil)[100][256]);

/* put the drawing surface onto a visible surface */
void scr_swap(void);

/* blits a sprite onto the invisible surface */
void scr_blit(SDL_Surface *s, int x, int y);

/* draws anything neccessary for the towergame */
void scr_drawall(long vert, long angle, long time, bool svisible, int subshape, int substart);

/* draws everything for the edit mode */
void scr_drawedit(long vert, long angle);

void scr_draw_bonus1(long horiz, long towerpos);
void scr_draw_bonus2(long horiz, long towerpos);
void scr_draw_submarine(long vert, long x, long number);
void scr_draw_fish(long vert, long x, long number);
void scr_draw_torpedo(long vert, long x);

#endif

