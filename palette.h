#ifndef PALETTE_H
#define PALETTE_H

#include <SDL/SDL.h>

/* the game has more or less 3 palettes, one for the tower game,
 one for the menu and one for the bonus game

 these palettes are managed in this module */

/* sets one color */
void pal_setpal(unsigned char nr, unsigned char r, unsigned char g, unsigned char b);

/* sets values that allow to change the colors for the tower and the cross */
void pal_setcrosspal(unsigned char nr, unsigned char r, unsigned char g, unsigned char b);
void pal_setbrickpal(unsigned char nr, unsigned char c1, unsigned char c2);

/* changes the colors of the tower and the cross */
void pal_setcrosscolors(unsigned char rk, unsigned char gk, unsigned char bk);
void pal_settowercolor(unsigned char r, unsigned char g, unsigned char b);

/* sets the actual colors */
void pal_colors();

/* makes all colors black */
void pal_black();

/* fades in and out */
void pal_fade_in();
void pal_fade_out();

/* saves the actual palette and restores it */
void pal_savepal(void **p);
void pal_restorepal(void *p);

/* sets the palette for this surface */
void pal_setpalette(SDL_Surface *s);

/* the real palette is only used for the display surface, all other
 surfaces use this palette */
void pal_setstdpalette(SDL_Surface *s);

/* makes all colors except the ones between as and ae darker */
void pal_darkening(int as, int ae);

/* calculates the indices for darker colors for the water */
void pal_calcdark();

/* returns an index of a coloer that is a little bit darker than
 the given color */
unsigned char pal_dark(unsigned char p);

/* returns the red green and blue values of the given color
 in the current palette */
char pal_red(unsigned char i);
char pal_green(unsigned char i);
char pal_blue(unsigned char i);

#endif
