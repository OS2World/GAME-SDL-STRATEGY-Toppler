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

#ifndef GAME_H
#define GAME_H

#include <SDL_types.h>

/* return values of towergame */
#define GAME_FINISHED 0
#define GAME_DIED     1
#define GAME_ABBORTED 2


/* load data */
void gam_init(void);

/* free data */
void gam_done(void);

/* initialize a completely new game, points to zero, ... */
void gam_newgame(void);

/* initializes the tower specific data structures */
void gam_loadtower(Uint8 tow);

/* leave toppler at the base of the tower */
void gam_arrival(void);


/* plays the towergame
 returns either
 GAME_FINISHED if the tower has been finished successfully
 GAME_DIED     if the toppler died
 GAME_ABBORTED if the game was abborted (ESC)*/
int gam_towergame(Uint8 &anglepos, Uint16 &resttime);

/* pick up the toppler at the base of the tower */
void gam_pick_up(Uint8 anglepos, Uint16 time);

#endif
