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

/* the main menu, return 0 for quit and
 bigger values for the missions */
void men_main(void);

/* highscores, after the game */
void men_highscore(unsigned long pt);

/* free graphics */
void men_done(void);

/* input line */
void men_input(char *s, int max_len, int xpos = -1,
               int ypos = (SCREENHEI  * 2) / 3,
               const char *allowed = NULL);

/* asks a yes/no question; return 0 if "no",
   1 if "yes" */
unsigned char men_yn(char *s, bool defchoice);

/* shows string s, waits either a certain time, (-1 = indefinitely),
   and if fire = 1 -> "press fire", if fire = 2 -> "press space" */
void men_info(char *s, long timeout = -1, int fire = 0);

#ifdef GAME_DEBUG_KEYS
void run_debug_menu(void);
#endif

/* Menu option flags */
typedef enum {
  MOF_NONE     = 0x00,
  MOF_PASSKEYS = 0x01,  /* Do keys get passed onto this option? */
  MOF_NOCENTER = 0x02   /* Do _not_ center this option txt */
} menuoptflags;

/* create a new menu */
struct _menusystem *new_menu_system(char *title, menuopt_callback_proc pr, 
				    int molen = 0, int ystart = 25);

/* add an option to the menu */
struct _menusystem *add_menu_option(struct _menusystem *ms, char *name, menuopt_callback_proc pr,
                SDLKey quickkey = SDLK_UNKNOWN, menuoptflags flags = MOF_NONE, int state = 0);

/* run the menu */
struct _menusystem *run_menu_system(struct _menusystem *ms);

/* free the menu */
void free_menu_system(struct _menusystem *ms);

/* sets the function that gets called whenever the background
   needs to be drawn in men_yn(), and men_info() */
void set_men_bgproc(menubg_callback_proc proc);

/* menu shown to user when he presses esc during play */
bool men_game(void);

#endif
