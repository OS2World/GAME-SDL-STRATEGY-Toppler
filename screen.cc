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
#include <math.h>

static SDL_Surface *display;

void
scr_savedisplaybmp(char *fname)
{
   SDL_SaveBMP(display, fname);
}

static Uint8 *slicedata, *battlementdata, *crossdata;

static int slicestart;
static int battlementstart;

static unsigned short robotsst, ballst, boxst, snowballst, starst, crossst,
         fishst, subst, torb;
static int topplerstart;

static unsigned short  step, elevatorsprite, stick;

/* table used to calculate the distance of an object from the center of the
 tower that is at x degrees on the tower */
static int sintab[TOWER_ANGLES];

/* this value added to the start of the animal sprites leads to
 the mirrored ones */
#define umkehr          37

/* the state of the flashing boxes */
static int boxstate;

static struct {
  int xs;
  int br;
  unsigned short s[3];
  Uint8 *data[3];
} doors[73];

static struct {
  unsigned short s;
  unsigned char width;
} fontchars[91];  //32..122

#define scrolllayers 3

static long layerwidth[scrolllayers];
static Uint16 layerimage[scrolllayers];

Uint8 towerpal[2*256];
Uint8 crosspal[2*256];

unsigned short scr_loadsprites(int num, int w, int h, int bits, bool sprite, const Uint8 *pal) {
  Uint16 erg = 0;
  Uint8 b;
  SDL_Surface *z;

  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | (sprite) ? SDL_SRCALPHA : 0,
                             2* w, 2* h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, (sprite) ? 0xFF000000 : 0);
  
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        b = arc_getbits(bits);
        if (sprite && (b == 0)) {
          ((Uint8 *)(z->pixels))[(2*y+0)*z->pitch+(2*x+0)*z->format->BytesPerPixel + 3] = 0;
          ((Uint8 *)(z->pixels))[(2*y+0)*z->pitch+(2*x+1)*z->format->BytesPerPixel + 3] = 0;
          ((Uint8 *)(z->pixels))[(2*y+1)*z->pitch+(2*x+0)*z->format->BytesPerPixel + 3] = 0;
          ((Uint8 *)(z->pixels))[(2*y+1)*z->pitch+(2*x+1)*z->format->BytesPerPixel + 3] = 0;
        } else {
          if (sprite) b--;
          for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++) {
              ((Uint8 *)(z->pixels))[(2*y+i)*z->pitch+(2*x+j)*z->format->BytesPerPixel + 0] = pal[b*3 + 2];
              ((Uint8 *)(z->pixels))[(2*y+i)*z->pitch+(2*x+j)*z->format->BytesPerPixel + 1] = pal[b*3 + 1];
              ((Uint8 *)(z->pixels))[(2*y+i)*z->pitch+(2*x+j)*z->format->BytesPerPixel + 2] = pal[b*3 + 0];
              ((Uint8 *)(z->pixels))[(2*y+i)*z->pitch+(2*x+j)*z->format->BytesPerPixel + 3] = 255;
            }
        }
      }

    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}

unsigned short scr_loadsprites_new(int num, int w, int h, bool sprite, const Uint8 *pal) {
  Uint16 erg = 0;
  Uint8 b, a;
  SDL_Surface *z;

  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | (sprite) ? SDL_SRCALPHA : 0,
                             w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, (sprite & use_alpha) ? 0xFF000000 : 0);

    if (sprite & !use_alpha)
      SDL_SetColorKey(z, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(z->format, 1, 1, 1));
  
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        b = arc_getbits(8);
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 0] = pal[b*3 + 2];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 1] = pal[b*3 + 1];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2] = pal[b*3 + 0];
        if (sprite) {
          a = arc_getbits(8);
          if (use_alpha)
            ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 3] = a;
          else {
            /* ok, this is the case where we have a sprite and don't want
             to use alpha blending, so we use normal sprites with key color
             instead, this is much faster. So if the pixel is more than 50% transparent
             make the whole pixel transparent by setting this pixel to the
             key color. if the pixel is not supoosed to be transparent
             we need to check if the pixel color is by accident the key color,
             if so we alter is slightly */
            if (a < 128) {
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 0] = 1;
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 1] = 1;
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2] = 1;
            }  else {
              if ((pal[b*3+2] == 1) && (pal[b*3+1] == 1) || (pal[b*3] == 1))
                ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2]++;
            }
          }
        }
      }

    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}


static unsigned short scr_gensprites(int num, int w, int h, bool sprite) {
  Uint16 erg = 0;
  SDL_Surface *z;

  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | (sprite) ? SDL_SRCALPHA : 0,
                             w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, (sprite) ? 0xFF000000 : 0);
  
    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}

static void scr_regensprites(Uint8 *data, SDL_Surface *z, int num, int w, int h, bool sprite, const Uint8 *pal) {
  Uint8 a, b;
  Uint32 datapos = 0;

  for (int t = 0; t < num; t++)
    for (int y = 0; y < h; y++)
      for (int x = 0; x < w; x++) {
        b = data[datapos++];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 0] = pal[b*3 + 2];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 1] = pal[b*3 + 1];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2] = pal[b*3 + 0];
        if (sprite) {
          a = data[datapos++];
          if (use_alpha) {
            ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 3] = a;
          } else {
            /* ok, this is the case where we have a sprite and don't want
             to use alpha blending, so we use normal sprites with key color
             instead, this is much faster. So if the pixel is more than 50% transparent
             make the whole pixel transparent by setting this pixel to the
             key color. if the pixel is not supoosed to be transparent
             we need to check if the pixel color is by accident the key color,
             if so we alter is slightly */
            if (a < 128) {
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 0] = 1;
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 1] = 1;
              ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2] = 1;
            }  else {
              if ((pal[b*3+2] == 1) && (pal[b*3+1] == 1) || (pal[b*3] == 1))
                ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2]++;
            }
          }
        }
      }
}

static void read_palette(Uint8 *pal) {
  Uint8 b;
  Uint32 res;
  arc_read(&b, 1, &res);

  arc_read(pal, (Uint32)b*3+3, &res);
}


/* loads all the graphics */
static void loadgraphics(void) {

  Uint32 res;
  unsigned char pal[3*256];
  int t;

  arc_assign(grafdat);

  arc_read(towerpal, 2*256, &res);

  for (t = 0; t < 4; t++)

  slicedata = (Uint8*)malloc(SPR_SLICESPRITES * SPR_SLICEWID * SPR_SLICEHEI);
  arc_read(slicedata, SPR_SLICESPRITES * SPR_SLICEWID * SPR_SLICEHEI, &res);

  battlementdata = (Uint8*)malloc(SPR_BATTLFRAMES * SPR_BATTLWID * SPR_BATTLHEI);
  arc_read(battlementdata, SPR_BATTLFRAMES * SPR_BATTLWID * SPR_BATTLHEI, &res);

  slicestart = scr_gensprites(SPR_SLICESPRITES, SPR_SLICEWID, SPR_SLICEHEI, false);
  battlementstart = scr_gensprites(SPR_BATTLFRAMES, SPR_BATTLWID, SPR_BATTLHEI, false);

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
      if (doors[t+36].br != 0) {
        doors[t+36].s[et] = scr_gensprites(1, doors[t+36].br, 16, false);
        doors[t+36].data[et] = (Uint8*)malloc(doors[t+36].br*16);
        arc_read(doors[t+36].data[et], doors[t+36].br*16, &res);
      } else {
        doors[t+36].s[et] = 0;
        doors[t+36].data[et] = NULL;
      }
  }

  scr_settowercolor(255, 0, 0);

  for (t = 0; t < 256; t++) {
    unsigned char c1, c2;

    arc_read(&c1, 1, &res);
    arc_read(&c2, 1, &res);

    pal[3*t] = c1;
    pal[3*t+1] = c2;
    pal[3*t+2] = c2;
  }

  step = scr_loadsprites_new(SPR_STEPFRAMES, SPR_STEPWID, SPR_STEPHEI, false, pal);
  elevatorsprite = scr_loadsprites_new(SPR_ELEVAFRAMES, SPR_ELEVAWID, SPR_ELEVAHEI, false, pal);
  stick = scr_loadsprites_new(1, SPR_STICKWID, SPR_STICKHEI, false, pal);

  arc_closefile();

  arc_assign(topplerdat);
  
  read_palette(pal);

  topplerstart = scr_loadsprites_new(74, SPR_HEROWID, SPR_HEROHEI, true, pal);

  arc_assign(spritedat);

  read_palette(pal);
  robotsst = scr_loadsprites_new(128, SPR_ROBOTWID, SPR_ROBOTHEI, true, pal);

  read_palette(pal);
  ballst = scr_loadsprites_new(2, SPR_ROBOTWID, SPR_ROBOTHEI, true, pal);

  read_palette(pal);
  boxst = scr_loadsprites_new(16, SPR_BOXWID, SPR_BOXHEI, true, pal);

  read_palette(pal);
  snowballst = scr_loadsprites_new(1, SPR_AMMOWID, SPR_AMMOHEI, true, pal);

  read_palette(pal);
  starst = scr_loadsprites_new(16, SPR_STARWID, SPR_STARHEI, true, pal);
  sts_init(starst + 10);

  arc_read(pal, 3*subcnt - 3, &res);
  fishst = scr_loadsprites(16, SPR_FISHWID, SPR_FISHHEI, 6, true, pal);
  subst = scr_loadsprites(8, SPR_SUBMWID, SPR_SUBMHEI, 6, true, pal);
  torb = scr_loadsprites(1, SPR_TORPWID, SPR_TORPHEI, 6, true, pal);

  arc_closefile();

  arc_assign(crossdat);

  Uint8 numcol = arc_getbits(8);

  for (t = 0; t < numcol + 1; t++) {
    crosspal[2*t] = arc_getbits(8);
    arc_getbits(8);
    crosspal[2*t+1] = arc_getbits(8);
  }

  crossdata = (Uint8*)malloc(120*SPR_CROSSWID*SPR_CROSSHEI*2);
  arc_read(crossdata, 120*SPR_CROSSWID*SPR_CROSSHEI*2, &res);

  crossst = scr_gensprites(120, SPR_CROSSWID, SPR_CROSSHEI, true);

  arc_closefile();
}

void scr_settowercolor(Uint8 r, Uint8 g, Uint8 b) {

  Uint8 pal[3*256];

  int t;
  int bw, gw, rw;

  for (t = 0; t < 256; t++) {
    rw = (int)r*towerpal[2*t+1] + (255-(int)r)*towerpal[2*t];
    gw = (int)g*towerpal[2*t+1] + (255-(int)g)*towerpal[2*t];
    bw = (int)b*towerpal[2*t+1] + (255-(int)b)*towerpal[2*t];

    rw /= 256;
    gw /= 256;
    bw /= 256;

    pal[3*t] = rw;
    pal[3*t+1] = gw;
    pal[3*t+2] = bw;
  }

  for (t = 0; t < SPR_SLICESPRITES; t++)
    scr_regensprites(slicedata + t*SPR_SLICEWID*SPR_SLICEHEI, spr_spritedata(slicestart + t), 1, SPR_SLICEWID, SPR_SLICEHEI, false, pal);

  for (t = 0; t < SPR_BATTLFRAMES; t++)
    scr_regensprites(battlementdata + t*SPR_BATTLWID*SPR_BATTLHEI, spr_spritedata(battlementstart + t), 1, SPR_BATTLWID, SPR_BATTLHEI, false, pal);

  for (t = -36; t <= 36; t++)
    for (int et = 0; et < 3; et++)
      if (doors[t+36].br != 0)
        scr_regensprites(doors[t+36].data[et], spr_spritedata(doors[t+36].s[et]), 1, doors[t+36].br, SPR_SLICEHEI, false, pal);
}

void scr_setcrosscolor(Uint8 rk, Uint8 gk, Uint8 bk) {

  Uint8 pal[256*3];

  int t, r, g, b;

  for (t = 0; t < 256; t++) {
    r = g = b = crosspal[2*t];

    r += ((int)crosspal[2*t+1] * rk) / 256;
    g += ((int)crosspal[2*t+1] * gk) / 256;
    b += ((int)crosspal[2*t+1] * bk) / 256;

    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 255)
      b = 255;

    pal[3*t+0] = r;
    pal[3*t+1] = g;
    pal[3*t+2] = b;
  }

  for (t = 0; t < 120; t++)
    scr_regensprites(crossdata + t*SPR_CROSSWID*SPR_CROSSHEI*2, spr_spritedata(crossst+t), 1, SPR_CROSSWID, SPR_CROSSHEI, true, pal);
}

static void loadfont(void) {

  unsigned char pal[256*3];
  Uint32 res;
  Uint8 c;
  bool first;
  int max_font_width = 0;
  int min_font_width = 0;
  int fontheight;

  arc_assign(fontdat);

  c = arc_getbits(8);
  arc_read(pal, c*3 + 3, &res);

  first = true;

  fontheight = arc_getbits(8);

  while (!arc_eof()) {
    arc_read(&c, 1, &res);

    if (!c) break;

    arc_read(&fontchars[c-32].width, 1, &res);
    fontchars[c-32].s = scr_loadsprites_new(1, fontchars[c-32].width, fontheight, true, pal);

    if (first) {
      max_font_width = min_font_width = fontchars[c-32].width;
      first = false;
    } else {
      if (fontchars[c-32].width < min_font_width) min_font_width = fontchars[c-32].width;
      if (fontchars[c-32].width > max_font_width) max_font_width = fontchars[c-32].width;
    }
  }

//  assert(min_font_width == FONTMINWID, "fontmin wrong");
//  assert(max_font_width == FONTMAXWID, "fontmax wrong");

  arc_closefile();
}

static void loadscroller(void) {

  bool alpha_sv = use_alpha;

  use_alpha = false;

  arc_assign(scrollerdat);

  Uint8 layers;
  Uint8 towerpos;
  Uint16 towersp_num;
  Uint16 towersp_den;
  unsigned char c;
  Uint32 res;
  Uint8 pal[3*256];

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

    read_palette(pal);

    layerimage[l] = scr_loadsprites_new(1, layerwidth[l], 480, l != 0, pal);
  }

  arc_closefile();

  use_alpha = alpha_sv;
}

void scr_init(void) {
  spr_init(1000);
  loadgraphics();
  loadfont();
  loadscroller();
  display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 32,
                             SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));

  /* initialize sinus table */
  for (int i = 0; i < TOWER_ANGLES; i++)
    sintab[i] = int(sin(i * 2 * M_PI / TOWER_ANGLES) * (TOWER_RADIUS + SPR_STEPWID / 2) + 0.5);
}

void scr_reinit() {
  display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 32,
                             SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
}

void scr_done(void) {
  spr_done();
}

static void cleardesk(void) {
  SDL_Rect r;
  r.w = SCREENWID;
  r.h = SCREENHEI;
  r.x = r.y = 0;
  SDL_FillRect(display, &r, 0);
}

/*
 angle: 0 means column 0 is in front
        8 means comumn 1 ...

 height: 0 means row 0 is in the middle
         4 means row 1 ...
*/
static void puttower(long angle, long height, long towerheight, int shift = 0) {

  /* calculate the blit position of the lowest slice considering the current
   * vertical position
   */
  int slice = 0;
  int ypos = SCREENHEI / 2 - SPR_SLICEHEI + height;

  /* now go up until we go over the top of the screen or reach the
   * top of the tower
   */
  while ((ypos > -SPR_SLICEHEI) && (slice < towerheight)) {

    /* if we are over the bottom of the screen, draw the slice */
    if (ypos < SCREENHEI)
      scr_blit(spr_spritedata(slicestart + (angle % SPR_SLICEANGLES)), (SCREENWID / 2) - (SPR_SLICEWID / 2) + shift, ypos);

    slice++;
    angle = (angle + (SPR_SLICEANGLES / 2)) % TOWER_ANGLES;
    ypos -= SPR_SLICEHEI;
  }
}

static void putbattlement(long angle, long height) {

  /* calculate the lower border position of the battlement */
  int upend = (SCREENHEI / 2) - (lev_towerrows() * SPR_SLICEHEI - height);

  /* if it's below the top of the screen, then blit the battlement */
  if (upend > 0)
    scr_blit(spr_spritedata((angle % SPR_BATTLFRAMES) + battlementstart),
             (SCREENWID / 2) - (SPR_BATTLWID / 2), upend - SPR_BATTLHEI);
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

  int x, y, t, z, v, index_z, index_t;

  height *= 4;

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

      index_z = z * display->pitch + (SCREENWID - 10) * display->format->BytesPerPixel;
      index_t = z * display->pitch;

      for(x = 0; x < 10; x++) {
        ((Uint8 *)(display->pixels))[index_z + 0] = 30;
        ((Uint8 *)(display->pixels))[index_z + 1] = 0;
        ((Uint8 *)(display->pixels))[index_z + 2] = 0;
        ((Uint8 *)(display->pixels))[index_t + 0] = 30;
        ((Uint8 *)(display->pixels))[index_t + 1] = 0;
        ((Uint8 *)(display->pixels))[index_t + 2] = 0;
        index_z += display->format->BytesPerPixel;
        index_t += display->format->BytesPerPixel;
      }

      index_z = z * display->pitch + v * display->format->BytesPerPixel;
      index_t = t * display->pitch;

      for(x = 0; x < SCREENWID; x++) {
        if ((x+v > 0) && (x+v < SCREENWID)) {
          ((Uint8 *)(display->pixels))[index_z + 0] = ((long)((Uint8 *)(display->pixels))[index_t + 0]) * 8 / 12 + 30;
          ((Uint8 *)(display->pixels))[index_z + 1] = ((long)((Uint8 *)(display->pixels))[index_t + 1]) * 8 / 12;
          ((Uint8 *)(display->pixels))[index_z + 2] = ((long)((Uint8 *)(display->pixels))[index_t + 2]) * 8 / 12;
        }
        index_z += display->format->BytesPerPixel;
        index_t += display->format->BytesPerPixel;
      }
    }
  }
  wavetime++;
}

int scr_textlength(const char *s, int chars) {
  int len = 0;
  int pos = 0;
  unsigned char c;

  while (s[pos] && (chars > 0)) {
    if (s[pos] == ' ') {
      len += FONTMINWID;
    } else {
      c = s[pos] - 32;
      if ((c < 91) && (fontchars[c].s != 0))
        len += fontchars[c].width + 3;
    }
    pos++;

    chars--;
  }

  return len - 1;
}

void scr_writetext_center(long y, const char *s) {
  scr_writetext ((SCREENWID - scr_textlength(s)) / 2, y, s);
}

void scr_writetext(long x, long y, const char *s) {
  int t = 0;
  unsigned char c;
  while (s[t]) {
    if (s[t] == ' ') {
      x += FONTMINWID;
      t++;
      continue;
    }

    c = s[t] - 32;
    if ((c < 91) && (fontchars[c].s != 0)) {
      scr_blit(spr_spritedata(fontchars[c].s), x, y);
      x += fontchars[c].width + 3;
    }
    t++;
  }
}

void scr_putbar(int x, int y, int br, int h, Uint8 colr, Uint8 colg, Uint8 colb, Uint8 alpha) {
  SDL_Rect r;
  r.w = br;
  r.h = h;
  r.x = x;
  r.y = y;
  SDL_FillRect(display, &r, SDL_MapRGBA(display->format, colr, colg, colb, alpha));
}

void
scr_putrect(int x, int y, int br, int h, Uint8 colr, Uint8 colg, Uint8 colb, Uint8 alpha)
{
  scr_putbar(x, y,      1     , h, colr, colg, colb, alpha);
  scr_putbar(x, y,      br    , 1, colr, colg, colb, alpha);
  scr_putbar(x + br, y, 1     , h, colr, colg, colb, alpha);
  scr_putbar(x, y + h , br + 1, 1, colr, colg, colb, alpha);
}

/* exchange active and inactive page */
void scr_swap(void) {
  SDL_UpdateRect(display, 0, 0, 0, 0);
}

void scr_blit(SDL_Surface *s, int x, int y) {
  SDL_Rect r;
  r.w = s->w;
  r.h = s->h;
  r.x = x;
  r.y = y;
  SDL_BlitSurface(s, NULL, display, &r);
}


/* draws the tower and the doors */
static void draw_tower(long vert, long angle) {

  puttower(angle, vert, lev_towerrows());

  int slice = 0;
  int ypos = SCREENHEI / 2 - SPR_SLICEHEI + vert;

  while (ypos > SCREENHEI) {
    slice++;
    ypos -= SPR_SLICEHEI;
  }

  while ((ypos > -SPR_SLICEHEI) && (slice < lev_towerrows())) {

    for (int col = 0; col < 16; col++) {

      int a = (col * 8 + angle + 36) % TOWER_ANGLES;

      if ((a > 72) || !doors[a].br)
        continue;

      if (lev_is_door(slice, col)) {
        if (lev_is_door_upperend(slice, col))
          scr_blit(spr_spritedata(doors[a].s[2]), (SCREENWID / 2) + doors[a].xs, ypos);
        else if (lev_is_door_upperend(slice + 1, col))
          scr_blit(spr_spritedata(doors[a].s[1]), (SCREENWID / 2) + doors[a].xs, ypos);
        else
          scr_blit(spr_spritedata(doors[a].s[0]), (SCREENWID / 2) + doors[a].xs, ypos);
      }
    }

    slice++;
    ypos -= SPR_SLICEHEI;
  }
}

static void draw_tower_editor(long vert, long angle, int state) {

  puttower(angle, vert, lev_towerrows());


  int slice = 0;
  int ypos = SCREENHEI / 2 - SPR_SLICEHEI + vert;

  while (ypos > SCREENHEI) {
    slice++;
    ypos -= SPR_SLICEHEI;
  }

  while ((ypos > -SPR_SLICEHEI) && (slice < lev_towerrows())) {

    for (int col = 0; col < 16; col++) {

      int a = (col * 8 + angle + 36) % TOWER_ANGLES;

      if ((a > 72) || !doors[a].br)
        continue;

      if (lev_is_door(slice, col)) {
        if (lev_is_targetdoor(slice, col) && (state & 1)) continue;
        if (lev_is_door_upperend(slice, col))
          scr_blit(spr_spritedata(doors[a].s[2]), (SCREENWID / 2) + doors[a].xs, ypos);
        else if (lev_is_door_upperend(slice + 1, col))
          scr_blit(spr_spritedata(doors[a].s[1]), (SCREENWID / 2) + doors[a].xs, ypos);
        else
          scr_blit(spr_spritedata(doors[a].s[0]), (SCREENWID / 2) + doors[a].xs, ypos);
      }
    }

    slice++;
    ypos -= SPR_SLICEHEI;
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
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h);

    break;

  case 0x81:
  case 0x91:
  case 0xb1:
    scr_blit(spr_spritedata((angle % SPR_STEPFRAMES) + step), x - (SPR_STEPWID / 2), h);

    break;

  case 0x80:
  case 0x84:
  case 0x8c:
    scr_blit(spr_spritedata(stick), x - (SPR_STICKWID / 2), h);

    break;

  case 0x82:
    scr_blit(spr_spritedata(boxst + boxstate), x - (SPR_BOXWID / 2), h);

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
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h - (state % 4));
    break;
  case 0x0c:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h - SPR_SLICEHEI/2 + abs(state - 8));
    break;
  case 0x08:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h + (state % 4));
    break;
  case 0x81:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2), h);
    break;
  case 0x91:
    if (state & 1)
      scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2), h);
    break;
  case 0xb1:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2) + state % 4, h);
    break;

  case 0x80:
    scr_blit(spr_spritedata(stick), x - (SPR_STICKWID / 2), h);
    break;

  case 0x82:
    scr_blit(spr_spritedata(boxst + boxstate), x - (SPR_BOXWID / 2), h);
    break;

  case 0x10:
    scr_blit(spr_spritedata(ballst + 1), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2);
    break;
  case 0x20:
    scr_blit(spr_spritedata(ballst), x - (SPR_ROBOTWID / 2) + state / 2, h - SPR_ROBOTHEI/2);
    break;
  case 0x30:
    scr_blit(spr_spritedata(ballst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2);
    break;
  case 0x40:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2 + abs(state - 8));
    break;
  case 0x50:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI + abs(state - 8) * 2);
    break;
  case 0x60:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2) + abs(state - 8), h - SPR_SLICEHEI/2);
    break;
  case 0x70:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2) + 2 * abs(state - 8), h - SPR_SLICEHEI/2);
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

/* vert is the vertical position of the tower
 * a is the angle on the tower to be drawn 0 front, 32 right, ...
 * angle is the angle of the tower, 0 column 0 in front, 8, column 1, ...
 * hs, he are start and ending rows to be drawn
 */
static void putthings(long vert, long a, long angle) {

  /* ok, at first lets check if there is a column right at the
   angle to be drawn */
  if (((a - angle) & 0x7) == 0) {

    /* yes there is one, find out wich one */
    int col = ((a - angle) / TOWER_STEPS_PER_COLUMN) & (TOWER_COLUMNS - 1);

    /* calc the x pos where the thing has to be drawn */
    int x = sintab[a % TOWER_ANGLES] + (SCREENWID/2);

    int slice = 0;
    int ypos = SCREENHEI / 2 - SPR_SLICEHEI + vert;

    while ((ypos > -SPR_SLICEHEI) && (slice < lev_towerrows())) {
  
      /* if we are over the bottom of the screen, draw the slice */
      if (ypos < SCREENHEI)
        putcase(lev_tower(slice, col), x, ypos);
  
      slice++;
      ypos -= SPR_SLICEHEI;
    }
  }

  /* and now check for robots to be drawn */
  for (int rob = 0; rob < 4; rob++) {

    /* if the the current robot as active and not the cross */
    if (rob_kind(rob) != OBJ_KIND_NOTHING && rob_kind(rob) != OBJ_KIND_CROSS) {

      /* ok calc the angle the robots needs to be drawn at */
      int rob_a = (rob_angle(rob) - 4 + angle) & (TOWER_ANGLES - 1);

      /* check if the robot is "inside" the current column */
      if (rob_a > a - 2 && rob_a <= a + 2)
        putrobot(rob_kind(rob), rob_time(rob),
                 sintab[rob_a], SCREENHEI / 2 + vert - rob_vertical(rob) * 4);
    }
  }
}

static void putthings_editor(long vert, long a, long angle, int state) {

  /* ok, at first lets check if there is a column right at the
   angle to be drawn */
  if (((a - angle) & 0x7) == 0) {

    /* yes there is one, find out wich one */
    int col = ((a - angle) / TOWER_STEPS_PER_COLUMN) & (TOWER_COLUMNS - 1);

    /* calc the x pos where the thing has to be drawn */
    int x = sintab[a % TOWER_ANGLES] + (SCREENWID/2);

    int slice = 0;
    int ypos = SCREENHEI / 2 - SPR_SLICEHEI + vert;

    while ((ypos > -SPR_SLICEHEI) && (slice < lev_towerrows())) {
  
      /* if we are over the bottom of the screen, draw the slice */
      if (ypos < SCREENHEI)
        putcase_editor(lev_tower(slice, col), x, ypos, state);
  
      slice++;
      ypos -= SPR_SLICEHEI;
    }
  }
}

/* draws everything behind the tower */
static void draw_behind(long vert, long angle)
{
  for (int a = 1; a < 32; a ++) {
    putthings(vert, 64 - a, angle);
    putthings(vert, 64 + a, angle);
  }
}


static void draw_behind_editor(long vert, long angle, int state)
{
  for (int a = 0; a < 32; a ++) {
    putthings_editor(vert, 64 - a, angle, state);
    putthings_editor(vert, 64 + a, angle, state);
  }
}

/* draws everything in front of the tower */
static void draw_bevore(long  vert, long angle)
{
  for (int a = 0; a < 32; a ++) {
    putthings(vert, 32 - a, angle);
    putthings(vert, 96 + a, angle);
  }
  putthings(vert, 0, angle);
}

static void draw_bevore_editor(long  vert, long angle, int state)
{
  for (int a = 0; a < 32; a ++) {
    putthings_editor(vert, 32 - a, angle, state);
    putthings_editor(vert, 96 + a, angle, state);
  }
  putthings_editor(vert, 0, angle, state);
}

/* draws the cross that moves to and fro over the screen */
static void putkreuz(long vert)
{
  long i, y;

  for (int t = 0; t < 4; t++) {
    if (rob_kind(t) == OBJ_KIND_CROSS) {
      i = (rob_angle(t) - 60) * 5;
      y = (vert - rob_vertical(t)) * 4 + (SCREENHEI / 2) - SPR_CROSSHEI;
      if (y > -SPR_CROSSHEI && y < SCREENHEI)
        scr_blit(spr_spritedata(crossst + labs(rob_time(t)) % 120), i + (SCREENWID/2) - (SPR_CROSSWID/2), y);

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
  scr_writetext(5, 40, s);
}

void scr_drawall(long vert,
                 long angle,
                 long time,
                 bool svisible,
                 int subshape,
                 int substart
                ) {

  cleardesk();

  sts_blink();
  sts_draw();
  draw_behind(vert * 4, angle);
  draw_tower(vert * 4, angle);
  draw_bevore(vert * 4, angle);

  if (snb_exists())
    scr_blit(spr_spritedata(snowballst),
             sintab[(snb_anglepos() + angle) % TOWER_ANGLES] + (SCREENWID / 2) - (SPR_HEROWID - SPR_AMMOWID),
             ((vert - snb_verticalpos()) * 4) + (SCREENHEI / 2) - SPR_AMMOHEI);

  if (top_visible()) {
      scr_blit(spr_spritedata(topplerstart + top_shape() +
                              ((top_look_left()) ?  umkehr : 0)),
               (SCREENWID / 2) - (SPR_HEROWID / 2),
               (vert - top_verticalpos()) * 4 + (SCREENHEI / 2) - SPR_HEROHEI);

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

  putbattlement(angle, vert * 4);

  putwater(vert);

  draw_data(time);

  boxstate = (boxstate + 1) & 0xf;
}

void scr_drawedit(long vpos, long apos) {

  long t;
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

  t = (apos - angle) & (TOWER_ANGLES - 1);

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

  draw_behind_editor(vert * 4, angle, boxstate);
  draw_tower_editor(vert * 4, angle, boxstate);
  draw_bevore_editor(vert * 4, angle, boxstate);

  putbattlement(angle, vert * 4);

  putwater(vert);

  if (boxstate & 1) {
    scr_putrect((SCREENWID / 2) - (32 / 2), (SCREENHEI / 2) - 16, 32, 16, boxstate * 0xf, boxstate * 0xf, boxstate * 0xf, 128);
  }


  char s[20];
  sprintf(s, "%u", lev_towertime());
  scr_writetext_center(5, s);

  boxstate = (boxstate + 1) & 0xf;
}

static void put_scrollerlayer(long horiz, int layer) {
  horiz %= layerwidth[layer];
  scr_blit(spr_spritedata(layerimage[layer]), -horiz, 0);
  if (horiz + SCREENWID > layerwidth[layer])
    scr_blit(spr_spritedata(layerimage[layer]), layerwidth[layer] - horiz, 0);
}

void scr_draw_bonus1(long horiz, long towerpos) {
  put_scrollerlayer(1*horiz/2, 0);
  put_scrollerlayer(1*horiz/1  , 1);

  puttower(0, SCREENHEI/2, SCREENHEI, towerpos);
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
