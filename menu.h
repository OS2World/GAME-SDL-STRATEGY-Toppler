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

#ifndef MENU_H
#define MENU_H

#include "decl.h"

#include <SDL_types.h>
#include <SDL_keyboard.h>

/* the menu and the highscores */

/* menu option callback function. gets called with the
   menusystem as it's parameter, and should return a
   string describing this option. If the parameter
   is null, then this is called just to get the
   string back. */
typedef char *FDECL((*menuopt_callback_proc), (void *));

/* menu background callback procedure. this should draw the
   background screen for the menu. */
typedef void FDECL((*menubg_callback_proc), (void));

/* load graphics */
void men_init(void);

/* the main menu */
void men_main(void);

/* highscores, after the game
 * pt = points, 
 * twr = tower reached, -1 = mission finished
 */
void men_highscore(unsigned long pt, int twr);

/* free graphics */
void men_done(void);

/* input line; asks the user for a string.
 * 
 * This function returns immediately, and the return
 * value tells whether the user finished editing the string.
 */
bool men_input(char *origs, int max_len, int xpos = -1,
               int ypos = (SCREENHEI  * 2) / 3,
               const char *allowed = NULL);

/* asks a yes/no question; return 0 if "no",
   1 if "yes" */
unsigned char men_yn(char *s, bool defchoice);

/* shows string s, waits a certain time, (-1 = indefinitely),
   and if fire = 1 -> "press fire", if fire = 2 -> "press space" */
void men_info(char *s, long timeout = -1, int fire = 0);

#ifdef GAME_DEBUG_KEYS
void run_debug_menu(void);
#endif

/* Menu option flags */
typedef enum {
  MOF_NONE     = 0x00,
  MOF_PASSKEYS = 0x01,  /* Do keys get passed onto this option? */
  MOF_LEFT     = 0x02,  /* Option string is left justified */
  MOF_RIGHT    = 0x04   /* Option string is right justified */
} menuoptflags;

/* create a new menu */

/* sets the function that gets called whenever the background
   needs to be drawn in men_yn(), and men_info() */
void set_men_bgproc(menubg_callback_proc proc);

/* menu shown to user when he presses esc during play */
bool men_game(void);

#endif
