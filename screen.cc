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

#include "screen.h"

#include "palette.h"
#include "archi.h"
#include "sprites.h"
#include "robots.h"
#include "stars.h"
#include "points.h"
#include "toppler.h"
#include "snowball.h"
#include "level.h"
#include "decl.h"

#include <string.h>
#include <stdlib.h>

SDL_Surface *display;

static SDL_Surface *second;

void
scr_savedisplaybmp(char *fname)
{
   SDL_SaveBMP(display, fname);
}

static int slicestart;
static int battlementstart;

static unsigned short robotsst, ballst, boxst, snowballst, starst, crossst,
         fishst, subst, torb;
static int topplerstart;

static unsigned short  step, elevatorsprite, stick;

/* table used to calculate the distance of an object from the center of the
 tower that is at x degrees on the tower */
static long sintab[189] = {
  -11, -14, -17, -20, -22, -25, -27, -30, -32, -35, -37, -39, -41, -43, -45,
  -47, -48, -50, -51, -52, -54, -55, -56, -56, -57, -57, -58, -58, -58, -58,
  -58, -57, -57, -56, -56, -55, -54, -52, -51, -50, -48, -47, -45, -43, -41,
  -39, -37, -35, -32, -30, -27, -25, -22, -20, -17, -14, -11, -9, -6, -3, 0,
  3, 6, 9, 11, 14, 17, 20, 22, 25, 27, 30, 32, 35, 37, 39, 41, 43, 45, 47, 48,
  50, 51, 52, 54, 55, 56, 56, 57, 57, 58, 58, 58, 58, 58, 57, 57, 56, 56, 55,
  54, 52, 51, 50, 48, 47, 45, 43, 41, 39, 37, 35, 32, 30, 27, 25, 22, 20, 17,
  14, 11, 9, 6, 3, 0, -3, -6, -9, -11, -14, -17, -20, -22, -25, -27, -30, -32,
  -35, -37, -39, -41, -43, -45, -47, -48, -50, -51, -52, -54, -55, -56, -56,
  -57, -57, -58, -58, -58, -58, -58, -57, -57, -56, -56, -55, -54, -52, -51,
  -50, -48, -47, -45, -43, -41, -39, -37, -35, -32, -30, -27, -25, -22, -20,
  -17, -14, -11, -9, -6, -3, 0
};

/* this value added to the start of the animal sprites leads to
 the mirrored ones */
#define umkehr          37

/* the state of the flashing boxes */
static int boxstate;

static struct {
  int xs;
  int br;
  unsigned short s[3];
} doors[73];

static struct {
  unsigned short s;
  unsigned char width;
} fontchars[91];  //32..122

#define scrolllayers 3

static long layerwidth[scrolllayers];
static SDL_Surface *layerimage[scrolllayers];

unsigned short scr_loadsprites(int num, int w, int h, int bits, int colstart, bool sprite) {
  unsigned short erg = 0;
  unsigned char b;
  SDL_Surface *z;


  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                             w, h, 8, 0, 0, 0, 0);
    pal_setstdpalette(z);
    if (sprite) SDL_SetColorKey(z, SDL_SRCCOLORKEY, 0);
  
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        b = arc_getbits(bits);
        if (sprite) {
          if (b)
            b += colstart -1;
        } else
          b += colstart;
        ((char *)(z->pixels))[y*z->pitch+x] = b;
      }

    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}

/* loads a palette and sets the colors */
static void loadcolors(int cnt, int colstart) {
  unsigned char r, g, b;

  for (int t = 0; t < cnt - 1; t++) {
    r = arc_getbits(8);
    g = arc_getbits(8);
    b = arc_getbits(8);
    pal_setpal(colstart + t, r, g, b, pal_towergame);
    pal_setpal(colstart + t, r, g, b, pal_bonusgame);
  }
}

/* loads all the graphics */
static void loadgraphics(void) {

  Uint32 res;
  unsigned char pal[192];
  int t;

  unsigned char buffer[4000];

  arc_assign(grafdat);

  for (t = 0; t < brickcnt; t++) {
    unsigned char c1, c2;

    arc_read(&c1, 1, &res);
    arc_read(&c2, 1, &res);

    pal_setbrickpal(t, c1, c2);
  }

  slicestart = scr_loadsprites(SPR_SLICESPRITES, SPR_SLICEWID, SPR_SLICEHEI, 4, brickcol, false);
  battlementstart = scr_loadsprites(SPR_BATTLFRAMES, SPR_BATTLWID, SPR_BATTLHEI, 4, brickcol, false);

  for (t = -36; t <= 36; t++) {

    Uint8 tmp;

    arc_read(&tmp, 1, &res);
    doors[t+36].xs = tmp;
    arc_read(&tmp, 1, &res);
    doors[t+36].xs |= ((Uint16)tmp) << 8;;

    arc_read(&tmp, 1, &res);
    doors[t+36].br = tmp;
    arc_read(&tmp, 1, &res);
    doors[t+36].br |= ((Uint16)tmp) << 8;;

    for (int et = 0; et < 3; et++)
      if (doors[t+36].br != 0)
        doors[t+36].s[et] = scr_loadsprites(1, doors[t+36].br, 8, 4, brickcol, false);
      else
        doors[t+36].s[et] = 0;
  }

  arc_read(&pal[0], envirocnt*2, &res);
  for (t = 0; t < envirocnt; t++)
    pal_setpal(envirocol + t, pal[2*t+1], pal[2*t], pal[2*t], pal_towergame);

  step = scr_loadsprites(SPR_STEPFRAMES, SPR_STEPWID, SPR_STEPHEI, 4, envirocol, false);
  elevatorsprite = scr_loadsprites(SPR_ELEVAFRAMES, SPR_ELEVAWID, SPR_ELEVAHEI, 4, envirocol, false);
  stick = scr_loadsprites(1, SPR_STICKWID, SPR_STICKHEI, 4, envirocol, false);


  arc_closefile();

  arc_assign(topplerdat);
  
  arc_read(pal, 8*3, &res);
  for (t = 0; t < 8; t++)
    pal_setpal(topplercol + t, pal[t * 3], pal[t * 3 + 1], pal[t * 3 + 2], pal_towergame);

  topplerstart = scr_loadsprites(74, SPR_HEROWID, SPR_HEROHEI, 4, topplercol, true);

  arc_assign(spritedat);

  loadcolors(robotscnt, robotscol);
  robotsst = scr_loadsprites(128, SPR_ROBOTWID, SPR_ROBOTHEI, 6, robotscol, true);

  loadcolors(ballcnt, ballcol);
  ballst = scr_loadsprites(2, SPR_ROBOTWID, SPR_ROBOTHEI, 5, ballcol, true);

  loadcolors(boxcnt, boxcol);
  boxst = scr_loadsprites(16, SPR_BOXWID, SPR_BOXHEI, 4, boxcol, true);

  loadcolors(snowballcnt, snowballcol);
  snowballst = scr_loadsprites(1, SPR_AMMOWID, SPR_AMMOHEI, 3, snowballcol, true);

  loadcolors(starcnt, starcol);
  starst = scr_loadsprites(16, SPR_STARWID, SPR_STARHEI, 3, starcol, true);
  sts_init(starst + 10);

  loadcolors(subcnt, subcol);
  fishst = scr_loadsprites(16, SPR_FISHWID, SPR_FISHHEI, 6, subcol, true);
  subst = scr_loadsprites(8, SPR_SUBMWID, SPR_SUBMHEI, 6, subcol, true);
  torb = scr_loadsprites(1, SPR_TORPWID, SPR_TORPHEI, 6, subcol, true);

  arc_closefile();

  arc_assign(crossdat);

  for (t = 0; t < crosscnt; t++) {
    int r = arc_getbits(8);
    int g = arc_getbits(8);
    int b = arc_getbits(8);

    pal_setcrosspal(t, r, g, b);
  }

  crossst = scr_loadsprites(120, SPR_CROSSWID, SPR_CROSSHEI, 4, crosscol+1, true);

  arc_closefile();
}


static void loadfont(void) {

  unsigned char pal[fontcnt*3];
  Uint32 res;
  SDL_Surface *s;
  char c;
  bool first;
  int max_font_width;
  int min_font_width;

  arc_assign(fontdat);

  arc_read(pal, fontcnt*3, &res);

  for (int t = 0; t < fontcnt-1; t++) {
    pal_setpal(t+fontcol, pal[3*t], pal[3*t+1], pal[3*t+2], pal_towergame);
    pal_setpal(t+fontcol, pal[3*t], pal[3*t+1], pal[3*t+2], pal_menu);
    pal_setpal(t+fontcol, pal[3*t], pal[3*t+1], pal[3*t+2], pal_bonusgame);
  }

  first = true;

  while (!arc_eof()) {
    arc_read(&c, 1, &res);
    if (!c) break;

    arc_read(&fontchars[c-32].width, 1, &res);
    fontchars[c-32].s = scr_loadsprites(1, fontchars[c-32].width, 20, 4, fontcol, true);

    if (first) {
      max_font_width = min_font_width = fontchars[c-32].width;
      first = false;
    } else {
      if (fontchars[c-32].width < min_font_width) min_font_width = fontchars[c-32].width;
      if (fontchars[c-32].width > max_font_width) max_font_width = fontchars[c-32].width;
    }
  }

  assert(min_font_width == FONTMINWID, "fontmin wrong");
  assert(max_font_width == FONTMAXWID, "fontmax wrong");

  arc_closefile();
}

static void loadscroller(void) {
  int i;

  arc_assign(scrollerdat);

  Uint8 layers;
  Uint8 towerpos;
  Uint16 towersp_num;
  Uint16 towersp_den;
  unsigned char c;
  Uint32 res;

  arc_read(&layers, 1, &res);
  arc_read(&towerpos, 1, &res);

  arc_read(&c, 1, &res);
  towersp_num = ((Uint16)c) << 8;
  arc_read(&c, 1, &res);
  towersp_num += c;

  arc_read(&c, 1, &res);
  towersp_den = ((Uint16)c) << 8;
  arc_read(&c, 1, &res);
  towersp_den += c;


  Uint8 r, g, b;

  for (i = 0; i < 152; i++) {
    arc_read(&r, 1, &res);
    arc_read(&g, 1, &res);
    arc_read(&b, 1, &res);
    pal_setpal(16 + i, r, g, b, pal_bonusgame);
  }

  assert(layers == 3, "another value than 3 is not yet supported");

  for (int l = 0; l < layers; l++) {
    layerwidth[l] = 0;

    arc_read(&c, 1, &res);
    layerwidth[l] = ((int)c) << 8;
    arc_read(&c, 1, &res);
    layerwidth[l] += c;

    arc_read(&c, 1, &res);
    arc_read(&c, 1, &res);
    arc_read(&c, 1, &res);
    arc_read(&c, 1, &res);

    layerimage[l] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                                         layerwidth[l], 240, 8, 0, 0, 0, 0);
    SDL_SetColorKey(layerimage[l], SDL_SRCCOLORKEY, 0);
    pal_setstdpalette(layerimage[l]);
    arc_read((layerimage[l]->pixels), layerwidth[l]*240, &res);

    for (int i = 0; i < layerwidth[l]*240; i++)
      if (((char *)(layerimage[l]->pixels))[i])
        ((char *)(layerimage[l]->pixels))[i] += 16;
  }

  arc_closefile();
}

void scr_init(void) {
  spr_init(1000);
  loadgraphics();
  loadfont();
  loadscroller();
  if (doublescale)
    display = SDL_SetVideoMode(SCREENWID*2, SCREENHEI*2, 8,
                               SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
  else
    display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 8,
                               SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
  second = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREENWID, SCREENHEI, 8, 0, 0, 0, 0);
  pal_setstdpalette(second);
}

void scr_reinit() {
  if (doublescale)
    display = SDL_SetVideoMode(SCREENWID*2, SCREENHEI*2, 8,
                               SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
  else
    display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 8,
                               SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
  pal_colors(pal_menu);
}

void scr_done(void) {
  SDL_FreeSurface(second);
  spr_done();
}

static void cleardesk(void) {
  SDL_Rect r;
  r.w = SCREENWID;
  r.h = SCREENHEI;
  r.x = r.y = 0;
  SDL_FillRect(second, &r, 0);
}

static void puttower(long angle, long height, long towerheight, int shift = 0) {

  int w, upend, a;

  height *= 2;

  if (height < (SCREENHEI / 2))
    w = (SCREENHEI / 2) - SPR_SLICEHEI + height;
  else
    w =  SCREENHEI      - SPR_SLICEHEI + (height % SPR_SLICEHEI);

  upend = (SCREENHEI / 2) - (towerheight - height);

  a = ((height - w) / 2) & (SPR_SLICEANGLES / 2);

  if (w > (SCREENHEI - SPR_SLICEHEI))
    scr_blit(spr_spritedata(((angle + a) % SPR_SLICEANGLES) + slicestart), (SCREENWID / 2) - (SPR_SLICEWID / 2) + shift, w);

  while ((w >= 0) && (w >= upend)) {
    scr_blit(spr_spritedata(((angle + a) % SPR_SLICEANGLES) + slicestart), (SCREENWID / 2) - (SPR_SLICEWID / 2) + shift, w);
    w -= SPR_SLICEHEI;
    a += (SPR_SLICEANGLES / 2);
  }

  if ((w < 0) && (w > -SPR_SLICEHEI) && (w >= upend))
    scr_blit(spr_spritedata(((angle + a) % SPR_SLICEANGLES) + slicestart), (SCREENWID / 2) - (SPR_SLICEWID / 2), w);
}

static void putbattlement(long angle, long height) {

  int upend;

  height *= 2;

  upend = (SCREENHEI / 2) - (lev_towerrows() * SPR_SLICEHEI - height);

  if (upend > 0)
    scr_blit(spr_spritedata((angle % SPR_BATTLFRAMES) + battlementstart), (SCREENWID / 2) - (SPR_BATTLWID / 2), upend - SPR_BATTLHEI);
}

static void putwater(long height) {

  static const char waves[] = {
    4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4,
    4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
    3, 3 };

  static int wavetime = 0;

  int x, y, t, z, v;

  height *= 2;

  if (height < (SCREENHEI / 2)) {
    for (y = 1; y <= (SCREENHEI / 2) - height; y++) {
      t = (SCREENHEI / 2) + height - y - 1 - waves[(wavetime * 4 + y * 2) & 0x7f];
      if (t <= 0) t = 0;
      z = waves[(wavetime*5 + y) & 0x7f];
      if (abs(z - 4) > y) {
        if (z < 4) {
          v = 4-y;
          z = (SCREENHEI / 2) + height + y - 1;
        } else {
          v = 4+y;
          z = (SCREENHEI / 2) + height + y - 1;
        }
      } else {
        v = z;
        z = (SCREENHEI / 2) + height + y - 1;
      }

      for(x = 0; x < 10; x++) {
        ((unsigned char *)(second->pixels))[z * SCREENWID + x] = pal_dark(0);
        ((unsigned char *)(second->pixels))[z * SCREENWID + x + SCREENWID - 10] = pal_dark(0);
      }

      for(x = 0; x < SCREENWID; x++) {
        if ((x+v > 0) && (x+v < SCREENWID))
          ((unsigned char *)(second->pixels))[z * SCREENWID + x + v] =
            pal_dark(((unsigned char *)(second->pixels))[t * SCREENWID + x]);
      }
    }
  }
  wavetime++;
}

int scr_textlength(const char *s, int chars) {
  int len = 0;
  int pos = 0;
  unsigned char c;

  while (s[pos]) {
    if (s[pos] == ' ') {
      len += FONTWID;
    } else {
      c = s[pos] - 32;
      if ((c < 91) && (fontchars[c].s != 0))
        len += fontchars[c].width + 1;
    }
    pos++;
    if (pos == chars)
      break;
  }

  return len - 1;
}

void scr_writetext_center(long y, const char *s) {
  scr_writetext ((SCREENWID / 2) - (scr_textlength(s) / 2), y, s);
}

void scr_writetext(long x, long y, const char *s) {
  int t = 0;
  unsigned char c;
  while (s[t] != 0) {
    if (s[t] == ' ') {
      x += FONTWID;
      t++;
      continue;
    }

    c = s[t] - 32;
    if ((c < 91) && (fontchars[c].s != 0)) {
      scr_blit(spr_spritedata(fontchars[c].s), x, y);
      x += fontchars[c].width + 1;
    }
    t++;
  }
}

void scr_putbar(int x, int y, int br, int h, unsigned char col) {
  SDL_Rect r;
  r.w = br;
  r.h = h;
  r.x = x;
  r.y = y;
  SDL_FillRect(second, &r, col);
}

void
scr_putrect(int x, int y, int br, int h, unsigned char col = 0)
{
  scr_putbar(x, y,      1     , h, col);
  scr_putbar(x, y,      br    , 1, col);
  scr_putbar(x + br, y, 1     , h, col);
  scr_putbar(x, y + h , br + 1, 1, col);
}

/* exchange active and inactive page */
void scr_swap(void) {
  if (doublescale) {

    int p = 0;
    int q = 0;
    unsigned char i = ((unsigned char *)(second->pixels))[0];
    for (int y = 0; y < SCREENHEI; y++) {
      for (int x = 0; x < SCREENWID; x++) {
        ((char *)(display->pixels))[p++] = i;
        ((char *)(display->pixels))[p++] = i;
        q++;
        i = ((unsigned char *)(second->pixels))[q];
      }
      memmove(&((char *)(display->pixels))[p], &((char *)(display->pixels))[p - (SCREENWID*2)], SCREENWID*2);
      p += (SCREENWID * 2);
    }
  } else {
    int d = 0;
    int s = 0;
    for (int y = 0; y < SCREENHEI; y++) {
      memmove(&((char *)(display->pixels))[d], &((char *)(second->pixels))[s], SCREENWID);
      d += display->pitch;
      s += second->pitch;
    }
  }
  SDL_UpdateRect(display, 0, 0, 0, 0);
}

void scr_blit(SDL_Surface *s, int x, int y) {
  SDL_Rect r;
  r.w = s->w;
  r.h = s->h;
  r.x = x;
  r.y = y;
  SDL_BlitSurface(s, NULL, second, &r);
}


/* draws the tower and the doors */
static void draw_tower(long vert, long angle, long hs, long he) {

  long a, h, y, an;

  puttower(angle, vert, lev_towerrows() * 8);

  a = -36;
  while (((a - angle) & 0x7) != 0)
    a++;

  while (a <= 36) {
    an = ((a - angle) / 8) & 0xf;
    y = (vert * 2) - ((hs-1) * 8) + (SCREENHEI / 2) - 8;
    for (h = hs-1; h <= he; h++) {
      if (lev_is_door_upperend(h, an) && doors[a + 36].br) {
        scr_blit(spr_spritedata(doors[a + 36].s[2]), (SCREENWID / 2) - 160 + doors[a + 36].xs, y);
        scr_blit(spr_spritedata(doors[a + 36].s[1]), (SCREENWID / 2) - 160 + doors[a + 36].xs, y-8);
        scr_blit(spr_spritedata(doors[a + 36].s[0]), (SCREENWID / 2) - 160 + doors[a + 36].xs, y-16);
      }
      y -= 8;
    }
    a += 8;
  }
}

static void draw_tower_editor(long vert, long angle, long hs, long he, int state) {

  long a, h, y, an;

  puttower(angle, vert, lev_towerrows() * 8);

  a = -36;
  while (((a - angle) & 0x7) != 0)
    a++;

  while (a <= 36) {
    an = ((a - angle) / 8) & 0xf;
    y = (vert * 2) - (hs * 8) + (SCREENHEI / 2) - 8;
    for (h = hs; h <= he; h++) {
      if (lev_is_door(h, an) &&
          (!lev_is_targetdoor(h, an) || (state & 1))
          && doors[a + 36].br) {
        scr_blit(spr_spritedata(doors[a + 36].s[2]), (SCREENWID / 2) - 160 + doors[a + 36].xs, y);
      }
      y -= 8;
    }
    a += 8;
  }
}


/* draws something of the environment */
static void putcase(unsigned char w, long x, long h) {
  long angle = 0;
  switch (w) {

  case 0:
    /* blank case */
    break;

  case 0x85:
  case 0x89:
  case 0x8d:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2) + x, h);

    break;

  case 0x81:
  case 0x91:
  case 0xb1:
    scr_blit(spr_spritedata((angle % SPR_STEPFRAMES) + step), (SCREENWID / 2) - (SPR_STEPWID / 2) + x, h);

    break;

  case 0x80:
  case 0x84:
  case 0x8c:
    scr_blit(spr_spritedata(stick), (SCREENWID / 2) - (SPR_STICKWID / 2) + x, h);

    break;

  case 0x82:
    scr_blit(spr_spritedata(boxst + boxstate), x + (SCREENWID / 2) - (SPR_BOXWID / 2), h);

    break;
  }
}

static void putcase_editor(unsigned char w, long x, long h, int state) {
   long angle = 0;
  switch (w) {

  case 0:
    /* blank case */
    break;

  case 0x85:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2) + x, h - (state % 4));
    break;
  case 0x0c:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2) + x, h - 4 + abs(state - 8));
    break;
  case 0x08:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2) + x, h + (state % 4));
    break;
  case 0x81:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), (SCREENWID / 2) - (SPR_STEPWID / 2) + x, h);
    break;
  case 0x91:
    if (state & 1)
      scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), (SCREENWID / 2) - (SPR_STEPWID / 2) + x, h);
    break;
  case 0xb1:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), (SCREENWID / 2) - (SPR_STEPWID / 2) + x + state % 4, h);
    break;

  case 0x80:
    scr_blit(spr_spritedata(stick), (SCREENWID / 2) - (SPR_STICKWID / 2) + x, h);
    break;

  case 0x82:
    scr_blit(spr_spritedata(boxst + boxstate), x + (SCREENWID / 2) - (SPR_BOXWID / 2), h);
    break;

  case 0x10:
    scr_blit(spr_spritedata(ballst + 1), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2), h - 8);
    break;
  case 0x20:
    scr_blit(spr_spritedata(ballst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2) + state / 2, h - 8);
    break;
  case 0x30:
    scr_blit(spr_spritedata(ballst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2), h - 8);
    break;
  case 0x40:
    scr_blit(spr_spritedata(robotsst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2), h - 4 + abs(state - 8));
    break;
  case 0x50:
    scr_blit(spr_spritedata(robotsst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2), h - 8 + abs(state - 8) * 2);
    break;
  case 0x60:
    scr_blit(spr_spritedata(robotsst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2) + abs(state - 8), h);
    break;
  case 0x70:
    scr_blit(spr_spritedata(robotsst), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2) + 2 * abs(state - 8), h);
    break;
  }
}


/* draws a robot */
static void putrobot(int t, int m, long x, long h)
{
  int nr;

  if (h > (SCREENHEI + SPR_ROBOTHEI)) return;

  switch (t) {

    case OBJ_KIND_JUMPBALL:
      nr = ballst;
      break;
  
    case OBJ_KIND_FREEZEBALL:
    case OBJ_KIND_FREEZEBALL_FROZEN:
      nr = ballst + 1;
      break;
  
    case OBJ_KIND_DISAPPEAR:
      nr = starst + m * 2;
      break;
  
    case OBJ_KIND_APPEAR:
      nr = starst - m * 2 + 16;
      break;
  
    case OBJ_KIND_ROBOT_VERT:
    case OBJ_KIND_ROBOT_HORIZ:
      nr = robotsst + (lev_towernr() & 0x7) * 16 + ((m / 2) & 0xf);
      break;
  
    default:
      nr = 40;
      break;
  }

  scr_blit(spr_spritedata(nr), x + (SCREENWID / 2) - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI);
}

/* draws something of the tower */
static void putthings(long vert, long a, long angle, long hs, long he) {

  long x, y, h;

  if (((a - angle) & 0x7) == 0) {
    x = sintab[a + 60];
    y = (vert * 2) - (hs * 8) + 112;
    for (h = hs; h <= he; h++) {
      putcase(lev_tower(h, ((a - angle) / 8) & 0xf), x, y + (SCREENHEI / 2) - 120);
      y -= 8;
    }
  }

  for (h = 0; h <= 3; h++) {
    if (rob_kind(h) != OBJ_KIND_NOTHING && rob_kind(h) != OBJ_KIND_CROSS) {
      x = ((rob_angle(h) + angle + 56) & 0x7f) - 52;
      if (x > a + 4 && x <= a + 8)
        putrobot(rob_kind(h), rob_time(h),
                 sintab[x + 52], ((vert - rob_vertical(h)) * 2) + (SCREENHEI / 2));
    }
  }
}

static void putthings_editor(long vert, long a, long angle, long hs, long he, int state) {

  long x, y, h;

  x = sintab[a + 60];
  y = (vert * 2) - (hs * 8) + 112;
  for (h = hs; h <= he; h++) {
    putcase_editor(lev_tower(h, ((a - angle) / 8) & 0xf), x, y + (SCREENHEI / 2) - 120, state);
    y -= 8;
  }
}

/* draws everything behind the tower */
static void draw_behind(long vert, long angle, long hs, long he)
{
  long a;

  a = -60;
  while (a <= -37) {
    putthings(vert, a, angle, hs, he);
    a++;
  }

  a = 60;
  while (a >= 37) {
    putthings(vert, a, angle, hs, he);
    a--;
  }
}


static void draw_behind_editor(long vert, long angle, long hs, long he, int state)
{
  long a;

  a = -60;
  while ((a - angle) & 0x7) a++;

  while (a <= -37) {
    putthings_editor(vert, a, angle, hs, he, state);
    a += 8;
  }

  a = 60;
  while ((a - angle) & 0x7) a--;

  while (a >= 37) {
    putthings_editor(vert, a, angle, hs, he, state);
    a -= 8;
  }
}

/* draws everything in front of the tower */
static void draw_bevore(long  vert, long angle, long hs, long he)
{
  long a;

  a = -36;
  while (a <= -1) {
    putthings(vert, a, angle, hs, he);
    a ++;
  }

  a = 36;
  while (a >= 0) {
    putthings(vert, a, angle, hs, he);
    a --;
  }
}

static void draw_bevore_editor(long  vert, long angle, long hs, long he, int state)
{
  long a;

  a = -36;
  while ((a - angle) & 0x7) a++;

  while (a <= -1) {
    putthings_editor(vert, a, angle, hs, he, state);
    a += 8;
  }

  a = 36;
  while ((a - angle) & 0x7) a--;

  while (a >= 0) {
    putthings_editor(vert, a, angle, hs, he, state);
    a -= 8;
  }
}

/* draws the cross that moves to and fro over the screen */
static void putkreuz(long vert)
{
  long i, y;

  for (int t = 0; t <= 3; t++) {
    if (rob_kind(t) == OBJ_KIND_CROSS) {
      i = rob_angle(t) * 3 - 28;
      if ((i <= -SPR_CROSSWID) || (i >= SCREENWID))
        return;
      y = (vert - rob_vertical(t)) * 2 + (SCREENHEI / 2) - SPR_CROSSHEI;
      if (y > -SPR_CROSSHEI && y < SCREENHEI)
        scr_blit(spr_spritedata(crossst + labs(rob_time(t)) % 120), i + (SCREENWID / 2) - 160, y);

      return;
    }
  }
}

/* draws the points, time and lifes left */
static void draw_data(int time)
{
  char s[256];
  int t;

  if (time > 0) {
    sprintf(s, "%u", time);
    scr_writetext_center(5L, s);
  }

  sprintf(s, "%u", pts_points());
  scr_writetext(5L, 5L, s);

  *s = '\0';
  for (t = 1; t <= pts_lifes(); t++)
    sprintf(s + strlen(s), "%c", fonttoppler);
  scr_writetext(5L, 21L, s);
}

void scr_drawall(long vert,
                 long angle,
                 long time,
                 bool svisible,
                 int subshape,
                 int substart
                ) {

  long hs, he;

  cleardesk();

  hs = vert / 4 - ((SCREENHEI / 2) / 8 + 1);
  if (hs < 0) hs = 0;

  he = vert / 4 + ((SCREENHEI / 2) / 8 + 1);
  if (he > lev_towerrows()) he = lev_towerrows() - 1;

  sts_blink();
  sts_draw();
  draw_behind(vert, angle, hs, he);
  draw_tower(vert, angle, hs, he);
  draw_bevore(vert, angle, hs, he);

  if (snb_exists())
    scr_blit(spr_spritedata(snowballst),
             sintab[((snb_anglepos() + angle) & 0x7f) + 60] + (SCREENWID / 2) - (SPR_HEROWID - SPR_AMMOWID),
             ((vert - snb_verticalpos()) * 2) + (SCREENHEI / 2) - SPR_AMMOHEI);

  if (top_visible()) {
      scr_blit(spr_spritedata(topplerstart + top_shape() +
                              ((top_look_left()) ?  umkehr : 0)),
               (SCREENWID / 2) - (SPR_HEROWID / 2),
	       (vert - top_verticalpos()) * 2 + (SCREENHEI / 2) - SPR_HEROHEI);

    if (top_onelevator())
      scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2),
	       vert - top_verticalpos() + (SCREENHEI / 2));
  }

  if (svisible) {
    /* TODO: use SPR_SUBMxxx */
    scr_blit(spr_spritedata(subst + subshape),
             (SCREENWID / 2) - 45 + 16,
             (SCREENHEI / 2) + 12 - substart + 16);

  }

  putkreuz(vert);

  putbattlement(angle, vert);

  putwater(vert);

  draw_data(time);

  boxstate = (boxstate + 1) & 0xf;
}

void scr_drawedit(long vpos, long apos) {

  long hs, he, t;
  static long vert = 0, angle = 0;

  if (vpos != vert) {
    if (vpos > vert) {
      if (vpos > vert + 0x8)
        vert += 4;
      else
        vert += 1;
    } else {
      if (vpos < vert - 0x8)
        vert -= 4;
      else
        vert -= 1;
    }
  }

  apos &= 0x7f;

  t = (apos - angle) & 0x7f;

  if (t != 0) {
    if (t < 0x3f) {
      if (t > 0x8)
        angle += 4;
      else
        angle += 1;
    } else {
      if (t < 0x7f-0x8)
        angle -= 4;
      else
        angle -= 1;
    }
    angle &= 0x7f;
  }

  cleardesk();

  hs = vert / 4 - 16;
  if (hs < 0) hs = 0;

  he = vert / 4 + 16;
  if (he > lev_towerrows()) he = lev_towerrows() - 1;

  draw_behind_editor(vert, angle, hs, he, boxstate);
  draw_tower_editor(vert, angle, hs, he, boxstate);
  draw_bevore_editor(vert, angle, hs, he, boxstate);

  putbattlement(angle, vert);

  putwater(vert);

  if (boxstate & 1) {
//    scr_putbar((SCREENWID / 2) - (16 / 2), (SCREENHEI / 2) - 8, 16, 8, envirocol + boxstate);
     scr_putrect((SCREENWID / 2) - (16 / 2), (SCREENHEI / 2) - 8, 16, 8, envirocol + boxstate);
  }


  char s[20];
  sprintf(s, "%u", lev_towertime());
  scr_writetext_center(5, s);

  boxstate = (boxstate + 1) & 0xf;
}

static void put_scrollerlayer(long horiz, int layer) {
  horiz %= layerwidth[layer];
  scr_blit(layerimage[layer], -horiz, 0);
  if (horiz + SCREENWID > layerwidth[layer])
    scr_blit(layerimage[layer], layerwidth[layer] - horiz, 0);
}

void scr_draw_bonus1(long horiz, long towerpos) {
  put_scrollerlayer(1*horiz/2, 0);
  put_scrollerlayer(1*horiz/1  , 1);

  puttower(1, 60, SCREENHEI, towerpos);
}
void scr_draw_bonus2(long horiz, long towerpos) {
  put_scrollerlayer(2*horiz/1, 2);

  draw_data(-1);
}

void scr_draw_submarine(long vert, long x, long number) {
  scr_blit(spr_spritedata(subst+number), x, vert);
}

void scr_draw_fish(long vert, long x, long number) {
  scr_blit(spr_spritedata(fishst+number), x, vert);
}

void scr_draw_torpedo(long vert, long x) {
  scr_blit(spr_spritedata(torb), x, vert);
}
