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

#ifndef MENU_H
#define MENU_H

#include "decl.h"

/* the menu and the higscores */

/* load graphics */
void men_init(void);

/* the main menu, return 0 for quit and
 bigger values for the missions */
void men_main(void);

/* highscores, after the game */
void men_highscore(unsigned long pt);

/* free graphics */
void men_done(void);

/* input line */
void men_input(char *s, int max_len, int xpos = -1, int ypos = (SCREENHEI  * 2) / 3);

/* asks a yes/no question; return 0 if "no",
   1 if "yes" */
unsigned char men_yn(char *s, bool defchoice);

/* shows string s, waits either a certain time, (-1 = indefinitely),
   and if fire = 1 -> "press fire", if fire = 2 -> "press space" */
void men_info(char *s, long timeout = -1, int fire = 0);

#ifdef GAME_DEBUG_KEYS
void run_debug_menu(void);
#endif

typedef void FDECL((*callback_proc), (void));

/* sets the function that gets called whenever the background
   needs to be drawn in men_yn(), and men_info() */
void set_men_bgproc(callback_proc proc);

#endif
