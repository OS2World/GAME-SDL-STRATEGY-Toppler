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

#if (SYSTEM != SYS_WINDOWS)
#include <dirent.h>
#endif

/* screen width and height, in pixels. */
#define SCREENWID 640
#define SCREENHEI 480

/* font width and height, in pixels. */
#define FONTWID 24
#define FONTMINWID 10
#define FONTMAXWID 40
#define FONTHEI 40

/* the tower dimensions */
#define TOWER_SLICE_HEIGHT 16
#define TOWER_RADIUS 96
#define TOWER_COLUMNS 16  // must be a 2 ** x
#define TOWER_STEPS_PER_COLUMN 8
#define TOWER_ANGLES (TOWER_COLUMNS*TOWER_STEPS_PER_COLUMN)

/* title sprite "NEBULOUS" width and height, in pixels */
#define SPR_TITLEWID 602
#define SPR_TITLEHEI 90

/* star sprite size */
#define SPR_STARWID 16
#define SPR_STARHEI 16

/* size of one layer sprite of tower */
#define SPR_SLICEWID 192
#define SPR_SLICEHEI 16
#define SPR_SLICEANGLES 8
#define SPR_SLICEFRAMES 1
#define SPR_SLICESPRITES (SPR_SLICEANGLES * SPR_SLICEFRAMES)

/* size of the battlement sprite on top of the tower */
#define SPR_BATTLWID 144 * 2
#define SPR_BATTLHEI 24 * 2
#define SPR_BATTLFRAMES 8

/* size of platform sprite */
#define SPR_STEPWID 40
#define SPR_STEPHEI 15
#define SPR_STEPFRAMES 1

/* size of elevator sprite */
#define SPR_ELEVAWID 32
#define SPR_ELEVAHEI 15
#define SPR_ELEVAFRAMES 1

/* size of elevator stick / wall */
#define SPR_STICKWID 14
#define SPR_STICKHEI 15

/* size of shootable flashing box */
#define SPR_BOXWID 16
#define SPR_BOXHEI 16

/* size of our hero */
#define SPR_HEROWID 40
#define SPR_HEROHEI 40

/* size of hero's ammunition, the snowball */
#define SPR_AMMOWID 16
#define SPR_AMMOHEI 16

/* size of robot sprite */
#define SPR_ROBOTWID 32
#define SPR_ROBOTHEI 32

/* cross sprite size */
#define SPR_CROSSWID 32
#define SPR_CROSSHEI 32

/* size of the bonus game fish */
#define SPR_FISHWID  20
#define SPR_FISHHEI  20

/* submarine sprite size */
#define SPR_SUBMWID 50 // 60
#define SPR_SUBMHEI 39 // 17

/* submarine ammunition, torpedo */
#define SPR_TORPWID 16
#define SPR_TORPHEI 3


/*   define this if you want the bonus game to be accessible 
     from the main menu. */
#define HUNT_THE_FISH

#define TOWERNAMELEN 19

/* names of the different data files */
#define grafdat         "graphics"
#define fontdat         "font"
#define spritedat       "sprites"
#define topplerdat      "dude"
#define menudat         "menu"
#define crossdat        "cross"
#define titledat        "titles"
#define scrollerdat     "scroller"

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

/* the two special characters in the font */
#define fonttoppler     '#'
#define fontpoint       '\\'

#define FDECL(f,p) f p
#define SIZE(x) (int)(sizeof(x) / sizeof(x[0]))

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
extern bool use_water;
extern char editor_towername[TOWERNAMELEN+1];
extern bool use_alpha;

void load_config(void);
void save_config(void);

/* for errorcheking */
#define assert(cond,text) if (!(cond)) { printf("asserion failure: "text"\n"); exit(1); }


#define CONFIGDIR "/etc"
#define HISCOREDIR "/var/games/toppler"

#if (SYSTEM == SYS_WINDOWS)

#include "windows.h"

typedef struct dirent {
  char d_name[200];
} dirent;

#define strcasecmp stricmp

#endif

int alpha_scandir(const char *dir, struct dirent ***namelist,
            int (*select)(const struct dirent *));

#endif
