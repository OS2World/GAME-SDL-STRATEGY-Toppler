#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>

/* this modules handles nearly all the output onto the screen */


/* initializes the module, loads the graphics, sets up the display */
void scr_init();

/* frees graphics */
void scr_done();

/* loads a sprite, enters it into the sprice collection and returns the index */
unsigned short scr_loadsprites(int num, const int w, const int h, int colstart, bool sprite, bool descr);

/* all paint routines paint onto an invisible surface, to show this surface
 call swap */


/* writes some text onto the screen */
void scr_writetext(long x, long y, char *s);
/* centers the text horizontally */
void scr_writetext_center(long y, char *s);

/* draws a filles rectangle with color 0 */
void scr_putbar(int x, int y, int br, int h);

/* for the submarine game i intend to use a simpel voxel space (maybe)*/
void scr_putvoxel(long xpos, unsigned char (*grnd)[100][256],
                  unsigned char (*ceil)[100][256]);

/* put the drawing surface onto a visible surface */
void scr_swap();

/* blits a sprite onto the invisible surface */
void scr_blit(SDL_Surface *s, int x, int y);

/* draws anything neccessary for the towergame */
void scr_drawall(long vert, long angle, long time, bool svisible, int subshape, int substart);


void scr_draw_bonus1(long horiz, long towerpos);
void scr_draw_bonus2(long horiz, long towerpos);
void scr_draw_submarine(long vert, long x, long number);
void scr_draw_fish(long vert, long x, long number);
void scr_draw_torpedo(long vert, long x);

extern char scrollerpalette[168*3];

#endif

