#ifndef DECL_H
#define DECL_H

#include "config.h"

#include <stdio.h>

/* names of the different data files */
#define grafdat         "graphics.dat"
#define fontdat         "font.dat"
#define spritedat       "sprites.dat"
#define topplerdat      "toppler.dat"
#define menudat         "menu.dat"
#define crossdat        "cross.dat"
#define titledat        "titles.dat"

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


/* the two special caracters in the font */
#define fonttoppler     '#'
#define fontpoint       '*'

/* waits around 1/18 of a second */
extern void dcl_wait();

bool dcl_fileexists(char *n);

/* opens a file looking into the right directories */
FILE *file_open(char *name, char *par);

/* GAME PARAMETERS */
extern bool fullscreen;
extern bool nosound;

/* for errorcheking */
#define assert(cond,text) if (!(cond)) { printf(text); exit(1); }

#endif
