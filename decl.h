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

#ifndef DECL_H
#define DECL_H

#include "config.h"

#include <stdio.h>

/* names of the different data files */
#define grafdat         "graphics"
#define fontdat         "font"
#define spritedat       "sprites"
#define topplerdat      "toppler"
#define menudat         "menu"
#define crossdat        "cross"
#define titledat        "titles"
#define scrollerdat     "scroller"

#define hiscore         "nebulous.hsc"

/* the start number and the number of colors for the different objects */

/* bricks and battlement */
#define brickcol        0
#define brickcnt        16

/* the platforms, elevator, */
#define envirocol       (brickcol + brickcnt)            // == 16
#define envirocnt       16

/* thr animal */
#define topplercol      (envirocol + envirocnt)          // == 32
#define topplercnt      8

/* the different robots */
#define robotscol       (topplercol + topplercnt)        // == 40
#define robotscnt       64

/* the 2 different balls */
#define ballcol         (robotscol + robotscnt)          // == 104
#define ballcnt         32

/* the flashing boxes */
#define boxcol          (ballcol + ballcnt)              // == 136
#define boxcnt          16

/* the little snowball */
#define snowballcol    (boxcol + boxcnt)                // == 152
#define snowballcnt        8

/* the stars in background */
#define starcol         (snowballcol + snowballcnt)     // == 160
#define starcnt         8

/* the submarine */
#define subcol          (starcol + starcnt)              // == 168
#define subcnt          24

/* the turning cross that crosses the screen */
#define crosscol        (subcol + subcnt)                // == 192
#define crosscnt        16

/* used to calc dark colors for the water reflections */
#define shadowcol       (crosscol + crosscnt)            // == 208

/* the font */
#define fontcol         240
#define fontcnt         16

/* the IDs for the 3 different palettes */
#define pal_towergame 0
#define pal_menu      1
#define pal_bonusgame 2


/* the two special caracters in the font */
#define fonttoppler     '#'
#define fontpoint       '*'

/* waits around 1/18 of a second */
void dcl_wait(void);

/* opens files looking into the right directories */
FILE *open_data_file(char *name);
FILE *open_global_config_file(char *name);
FILE *open_local_config_file(char *name);
FILE *create_local_config_file(char *name);
FILE *open_highscore_file(char *name);
FILE *create_highscore_file(char *name);
FILE *open_local_data_file(char *name);
FILE *create_local_data_file(char *name);

/* GAME PARAMETERS */
extern bool fullscreen;
extern bool nosound;
extern bool doublescale;
extern bool use_water;

void load_config(void);
void save_config(void);

/* for errorcheking */
#define assert(cond,text) if (!(cond)) { printf("asserion failure: "text"\n"); exit(1); }


#define CONFIGDIR "/etc"
#define HISCOREDIR "/var/games/nebulous"

/* sunOS doesn't have this function so I need to implement by myself */
#if SELF_SCANDIR
int scandir(const char *dir, struct dirent ***namelist,
            int (*select)(const struct dirent *),
            int (*compar)(const void *, const void *));
int alphasort(const void *a, const void *b);
#endif

#endif
