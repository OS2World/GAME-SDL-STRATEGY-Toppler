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

#include "palette.h"

#include "decl.h"

#include <stdlib.h>

typedef struct {
  unsigned char r, g, b;
} color;

extern SDL_Surface *display;

static color pal[256][3];
static color crosspal[crosscnt];
static color towercolors[brickcnt];
static unsigned char dark[256];

static int blink_color = 0;
static int blink_color_dir = 1;

int get_blink_color(void) {
   blink_color += blink_color_dir;
   
   if (blink_color > 0x0d) blink_color_dir = -1;
   else if (blink_color < 1) blink_color_dir = 1;
   
   return blink_color;
}

void pal_setpal(unsigned char nr, unsigned char r, unsigned char g, unsigned char b, unsigned char p) {
  pal[nr][p].r = r;
  pal[nr][p].g = g;
  pal[nr][p].b = b;
}

void pal_setcrosspal(unsigned char nr, unsigned char r, unsigned char g, unsigned char b) {
  crosspal[nr].r = r;
  crosspal[nr].g = g;
  crosspal[nr].b = b;
}

void pal_setbrickpal(unsigned char nr, unsigned char c1, unsigned char c2) {
  towercolors[nr].r = c2;
  towercolors[nr].g = c1;
}

void pal_settowercolor(unsigned char r, unsigned char g, unsigned char b) {

  int t;
  int bw, gw, rw;

  for (t = 0; t < brickcnt; t++) {
    rw = (int)r*towercolors[t].r + (255-(int)r)*towercolors[t].g;
    gw = (int)g*towercolors[t].r + (255-(int)g)*towercolors[t].g;
    bw = (int)b*towercolors[t].r + (255-(int)b)*towercolors[t].g;

    rw /= 256;
    gw /= 256;
    bw /= 256;

    pal_setpal(t+brickcol, rw, gw, bw, pal_towergame);
    pal_setpal(t+brickcol, rw, gw, bw, pal_bonusgame);

    rw *= 8;
    gw *= 8;
    bw *= 8;

    rw /= 12;
    gw /= 12;
    bw /= 12;

    bw += 30;

    pal_setpal(t+shadowcol, rw, gw, bw, pal_towergame);
    pal_setpal(t+shadowcol, rw, gw, bw, pal_bonusgame);
  }
}

void pal_setcrosscolors(unsigned char rk, unsigned char gk, unsigned char bk) {

  int t, r, g, b;

  for (t = 0; t < crosscnt; t++) {
    r = crosspal[t].r;
    g = crosspal[t].r;
    b = crosspal[t].r;

    r += (crosspal[t].b * rk) / 256;
    g += (crosspal[t].b * gk) / 256;
    b += (crosspal[t].b * bk) / 256;

    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 255)
      b = 255;
      
    pal_setpal(t + crosscol, r, g, b, pal_towergame);  

    r *= 2;
    g *= 2;
    b *= 2;

    r /= 3;
    g /= 3;
    b /= 3;

    b += 30;
    
    pal_setpal(t + brickcnt + shadowcol, r, g, b, pal_towergame);
  }

  pal_calcdark(pal_towergame);
  pal_colors(pal_towergame);
}

void pal_colors(unsigned char palette)
{
  SDL_Color p[256];
  for (int i = 0; i < 256; i++) {
    p[i].r = pal[i][palette].r;
    p[i].g = pal[i][palette].g;
    p[i].b = pal[i][palette].b;
  }

  SDL_SetColors(display, p, 0, 256);
}

void pal_darkening(int as, int ae, unsigned char palette) {
  SDL_Color p[256];

  for(int i = 0; i < 256; i++) {
    if ((i < as) || (i > ae)) {
      p[i].r = pal[i][palette].r >> 1;
      p[i].g = pal[i][palette].g >> 1;
      p[i].b = pal[i][palette].b >> 1;
    } else {
      p[i].r = pal[i][palette].r;
      p[i].g = pal[i][palette].g;
      p[i].b = pal[i][palette].b;
    }
  }
  SDL_SetColors(display, p, 0, 256);
}

void pal_setstdpalette(SDL_Surface *s) {
  SDL_Color q[256];

  for (int i = 0; i < 256; i++) {
    q[i].r = 255-i;
    q[i].g = i;
    q[i].b = i;
  }

  SDL_SetColors(s, q, 0, 256);
}

void pal_setpalette(SDL_Surface *s) {
  SDL_Color q[256];

  for (int i = 0; i < 256; i++) {
    q[i].r = pal[i][1].r;
    q[i].g = pal[i][1].g;
    q[i].b = pal[i][1].b;
  }

  SDL_SetColors(s, q, 0, 256);
}

void pal_calcdark(unsigned char p) {
  int r, g, b, m;

  for (int t = 0; t < 256; t++) {
    r = pal[t][p].r;
    g = pal[t][p].g;
    b = pal[t][p].b;

    r *= 8;
    g *= 8;
    b *= 8;

    r /= 12;
    g /= 12;
    b /= 12;

    b += 30;

    m = 0;
    for (int z = 1; z < 256; z++)
      if ((abs(r-pal[z][p].r) + abs(b-pal[z][p].b) + abs(g-pal[z][p].g)) <
          (abs(r-pal[m][p].r) + abs(b-pal[m][p].b) + abs(g-pal[m][p].g)))
        m = z;

    dark[t] = m;
  }
}

unsigned char pal_dark(unsigned char p) { return dark[p]; }


