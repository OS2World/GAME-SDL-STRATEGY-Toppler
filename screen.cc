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
#include "keyb.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

static SDL_Surface *display;

static int color_ramp_radj = 3;
static int color_ramp_gadj = 5;
static int color_ramp_badj = 7;

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

/* this table is used for the waves of the water */
static Sint8 waves[0x80];

/* this value added to the start of the animal sprites leads to
 the mirrored ones */
#define mirror          37

/* the state of the flashing boxes */
static int boxstate;

static struct {
  int xstart;          // x start position
  int width;           // width of door
  unsigned short s[3]; // the sprite index for the 3 layers of the door
  Uint8 *data[3];      // the data for the 3 layers of the door (pixel info for recoloring)
} doors[73];

#define MAXCHARNUM 128

static struct {
  unsigned short s;
  unsigned char width;
} fontchars[MAXCHARNUM];

struct _scroll_layer {
  long width;
  int  num, den;
  Uint16 image;
};

static int num_scrolllayers;
static int sl_tower_depth,
           sl_tower_num,
           sl_tower_den;
static struct _scroll_layer *scroll_layers;

Uint8 towerpal[2*256];
Uint8 crosspal[2*256];

void color_ramp1(int *c, int *adj, int min, int max) {
  *c = *c + *adj;
  if (*c > max - abs(*adj)) {
    *c = max;
    *adj = -(*adj);
  } else if (*c < min + abs(*adj)) {
    *c = min;
    *adj = -(*adj);
  }
}

void scr_color_ramp(int *r, int *g, int *b) {
  color_ramp1(r, &color_ramp_radj, 1, 255);
  color_ramp1(g, &color_ramp_gadj, 1, 255);
  color_ramp1(b, &color_ramp_badj, 1, 255);
}

void
scr_savedisplaybmp(char *fname)
{
  SDL_SaveBMP(display, fname);
}

unsigned short scr_loadsprites(int num, int w, int h, bool sprite, const Uint8 *pal, bool use_alpha) {
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
        b = arc_getbyte();
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 0] = pal[b*3 + 2];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 1] = pal[b*3 + 1];
        ((Uint8 *)(z->pixels))[y*z->pitch+x*z->format->BytesPerPixel + 2] = pal[b*3 + 0];
        if (sprite) {
          a = arc_getbyte();
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


static unsigned short scr_gensprites(int num, int w, int h, bool sprite, bool use_alpha) {
  Uint16 erg = 0;
  SDL_Surface *z;

  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | (sprite && use_alpha) ? SDL_SRCALPHA : 0,
                             w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, (sprite && use_alpha) ? 0xFF000000 : 0);
  
    if (sprite & !use_alpha)
      SDL_SetColorKey(z, SDL_SRCCOLORKEY/* | SDL_RLEACCEL*/, SDL_MapRGB(z->format, 1, 1, 1));
    /* FIXME: SDL_RLEACCEL was buggy in my version of SDL, maybe it can be reincluded later on */

    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}

static void scr_regensprites(Uint8 *data, SDL_Surface *z, int num, int w, int h, bool sprite, const Uint8 *pal, bool use_alpha) {
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

void scr_read_palette(Uint8 *pal) {
  Uint8 b;
  Uint32 res;
  b = arc_getbyte();

  arc_read(pal, (Uint32)b*3+3, &res);
}


/* loads all the graphics */
static void loadgraphics(void) {

  Uint32 res;
  unsigned char pal[3*256];
  int t;

  arc_assign(grafdat);

  arc_read(towerpal, 2*256, &res);

  slicedata = (Uint8*)malloc(SPR_SLICESPRITES * SPR_SLICEWID * SPR_SLICEHEI);
  arc_read(slicedata, SPR_SLICESPRITES * SPR_SLICEWID * SPR_SLICEHEI, &res);

  battlementdata = (Uint8*)malloc(SPR_BATTLFRAMES * SPR_BATTLWID * SPR_BATTLHEI);
  arc_read(battlementdata, SPR_BATTLFRAMES * SPR_BATTLWID * SPR_BATTLHEI, &res);

  slicestart = scr_gensprites(SPR_SLICESPRITES, SPR_SLICEWID, SPR_SLICEHEI, false, false);
  battlementstart = scr_gensprites(SPR_BATTLFRAMES, SPR_BATTLWID, SPR_BATTLHEI, false, false);

  for (t = -36; t < 37; t++) {

    doors[t+36].xstart = arc_getword();
    doors[t+36].width = arc_getword();

    for (int et = 0; et < 3; et++)
      if (doors[t+36].width != 0) {
        doors[t+36].s[et] = scr_gensprites(1, doors[t+36].width, 16, false, false);
        doors[t+36].data[et] = (Uint8*)malloc(doors[t+36].width*16);
        arc_read(doors[t+36].data[et], doors[t+36].width*16, &res);
      } else {
        doors[t+36].s[et] = 0;
        doors[t+36].data[et] = NULL;
      }
  }

  scr_settowercolor(255, 0, 0);

  for (t = 0; t < 256; t++) {
    unsigned char c1, c2;

    c1 = arc_getbyte();
    c2 = arc_getbyte();

    pal[3*t] = c1;
    pal[3*t+1] = c2;
    pal[3*t+2] = c2;
  }

  step = scr_loadsprites(SPR_STEPFRAMES, SPR_STEPWID, SPR_STEPHEI, false, pal, false);
  elevatorsprite = scr_loadsprites(SPR_ELEVAFRAMES, SPR_ELEVAWID, SPR_ELEVAHEI, false, pal, false);
  stick = scr_loadsprites(1, SPR_STICKWID, SPR_STICKHEI, false, pal, false);

  arc_closefile();

  arc_assign(topplerdat);
  
  scr_read_palette(pal);

  topplerstart = scr_loadsprites(74, SPR_HEROWID, SPR_HEROHEI, true, pal, use_alpha_sprites);

  arc_assign(spritedat);

  scr_read_palette(pal);
  robotsst = scr_loadsprites(128, SPR_ROBOTWID, SPR_ROBOTHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  ballst = scr_loadsprites(2, SPR_ROBOTWID, SPR_ROBOTHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  boxst = scr_loadsprites(16, SPR_BOXWID, SPR_BOXHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  snowballst = scr_loadsprites(1, SPR_AMMOWID, SPR_AMMOHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  starst = scr_loadsprites(16, SPR_STARWID, SPR_STARHEI, true, pal, use_alpha_sprites);
  sts_init(starst + 9, NUM_STARS);

  scr_read_palette(pal);
  fishst = scr_loadsprites(32*2, SPR_FISHWID, SPR_FISHHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  subst = scr_loadsprites(31, SPR_SUBMWID, SPR_SUBMHEI, true, pal, use_alpha_sprites);

  scr_read_palette(pal);
  torb = scr_loadsprites(1, SPR_TORPWID, SPR_TORPHEI, true, pal, use_alpha_sprites);

  arc_closefile();

  arc_assign(crossdat);

  Uint8 numcol = arc_getbyte();

  for (t = 0; t < numcol + 1; t++) {
    crosspal[2*t] = arc_getbyte();
    arc_getbyte();
    crosspal[2*t+1] = arc_getbyte();
  }

  crossdata = (Uint8*)malloc(120*SPR_CROSSWID*SPR_CROSSHEI*2);
  arc_read(crossdata, 120*SPR_CROSSWID*SPR_CROSSHEI*2, &res);

  crossst = scr_gensprites(120, SPR_CROSSWID, SPR_CROSSHEI, true, use_alpha_sprites);

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
    scr_regensprites(slicedata + t*SPR_SLICEWID*SPR_SLICEHEI, spr_spritedata(slicestart + t), 1, SPR_SLICEWID, SPR_SLICEHEI, false, pal, false);

  for (t = 0; t < SPR_BATTLFRAMES; t++)
    scr_regensprites(battlementdata + t*SPR_BATTLWID*SPR_BATTLHEI, spr_spritedata(battlementstart + t), 1, SPR_BATTLWID, SPR_BATTLHEI, false, pal, false);

  for (t = -36; t < 37; t++)
    for (int et = 0; et < 3; et++)
      if (doors[t+36].width != 0)
        scr_regensprites(doors[t+36].data[et], spr_spritedata(doors[t+36].s[et]), 1, doors[t+36].width, SPR_SLICEHEI, false, pal, false);
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

  for (t = 0; t < 120; t++) {
    scr_regensprites(crossdata + t*SPR_CROSSWID*SPR_CROSSHEI*2,
                     spr_spritedata(crossst+t),
                     1, SPR_CROSSWID, SPR_CROSSHEI, true, pal, use_alpha_sprites);
  }
}

static void loadfont(void) {

  unsigned char pal[256*3];
  Uint8 c;
  int fontheight;

  arc_assign(fontdat);

  scr_read_palette(pal);

  fontheight = arc_getbyte();

  while (!arc_eof()) {
    c = arc_getbyte();

    if (!c || (c >= MAXCHARNUM)) break;

    fontchars[c].width = arc_getbyte();
    fontchars[c].s = scr_loadsprites(1, fontchars[c].width, fontheight, true, pal, use_alpha_font);
  }

  arc_closefile();
}

static void loadscroller(void) {

  arc_assign(scrollerdat);

  Uint8 layers;
  Uint8 towerpos;
  Uint8 pal[3*256];

  layers = arc_getbyte();

  num_scrolllayers = layers;

  assert(num_scrolllayers > 1, "Must have at least 2 scroll layers!");

  scroll_layers = (struct _scroll_layer *)malloc(sizeof(struct _scroll_layer)*layers);
  assert(scroll_layers, "Failed to alloc memory for bonus scroller!");
    
  towerpos = arc_getbyte();
    
  sl_tower_depth = towerpos;

  sl_tower_num = arc_getword();
  sl_tower_den = arc_getword();

  for (int l = 0; l < layers; l++) {

    scroll_layers[l].width = arc_getword();
    scroll_layers[l].num = arc_getword();
    scroll_layers[l].den = arc_getword();

    scr_read_palette(pal);

    scroll_layers[l].image = scr_loadsprites(1, scroll_layers[l].width, 480, l != 0, pal, use_alpha_layers);
  }

  arc_closefile();
}

static void load_sprites(void) {
  loadgraphics();
  loadfont();
  loadscroller();
}

static void free_memory(void) {
  int t;

  free(scroll_layers);

  free(slicedata);
  free(battlementdata);
  free(crossdata);

  for (t = -36; t < 37; t++)
    for (int et = 0; et < 3; et++)
      if (doors[t+36].data[et])
        free(doors[t+36].data[et]);
}

void scr_reload_sprites() {
  free_memory();
  load_sprites();
}

void scr_init(void) {
  spr_init(1000);

  load_sprites();

  display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 32,
                             SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));

  /* initialize sinus table */
  for (int i = 0; i < TOWER_ANGLES; i++)
    sintab[i] = int(sin(i * 2 * M_PI / TOWER_ANGLES) * (TOWER_RADIUS + SPR_STEPWID / 2) + 0.5);


  /* initialize wave table */
  for (int t = 0; t < 0x80; t++)
    waves[t] = (Sint8)(8 * (sin(t * 2.0 * M_PI / 0x7f)) +
      4 * (sin(t * 3.0 * M_PI / 0x7f+2)) +
      3 * (sin(t * 5.0 * M_PI / 0x7f+3)) + 0.5);

}

void scr_reinit() {
  display = SDL_SetVideoMode(SCREENWID, SCREENHEI, 32,
                             SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
}

void scr_done(void) {
  spr_done();
  free_memory();
  sts_done();
}

static void cleardesk(void) {
  SDL_Rect r;
  r.w = SCREENWID;
  r.h = SCREENHEI;
  r.x = r.y = 0;
  SDL_FillRect(display, &r, 0);
}

void scr_darkenscreen(void) {

  int x, y;

  if (!use_alpha_darkening)
    return;

  static SDL_Surface *s = NULL;

  if (!s) {
      s = SDL_CreateRGBSurface(SDL_HWSURFACE, 20, 20, 24, 0xff, 0xff00, 0xff0000, 0);
      SDL_SetColorKey(s, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0xff);

      for (y = 0; y < 20; y++)
	for (x = 0; x < 20; x++)
	  if ((y+x) & 1)
	    ((Uint8*)s->pixels)[y * s->pitch + x * s->format->BytesPerPixel] = 0x00;
          else
            ((Uint8*)s->pixels)[y * s->pitch + x * s->format->BytesPerPixel] = 0xff;

  }

  x = y = 0;

  while (y < SCREENHEI) {
    x = 0;
    while (x < SCREENWID) {
      scr_blit(s, x, y);
      x += 20;
    }
    y += 20;
  }
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

  static const char simple_waves[] = {
    4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4,
    4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
    3, 3 };


  static int wavetime = 0;

  height *= 4;

  if (height < (SCREENHEI / 2)) {

    if (use_waves) {

      int targetidx = ((SCREENHEI / 2) + height) * display->pitch;
      int source_line = (SCREENHEI / 2) + height - 1;
  
      for (int y = 0; y < (SCREENHEI / 2) - height; y++) {
  
        for (int x = 0; x < SCREENWID; x++) {
          Sint16 dx = waves[(x+y+12*wavetime) & 0x7f] + waves[2*x-y+11*wavetime & 0x7f];
          Sint16 dy = waves[(x-y+13*wavetime) & 0x7f] + waves[2*x-3*y-14*wavetime & 0x7f];
  
          dx = dx * y / (SCREENHEI/2);
          dy = dy * y / (SCREENHEI/2);
  
          Uint8 r, g, b;
  
          if ((x+dx < 0) || (x+dx > SCREENWID) || (source_line+dy < 0)) {
            r = g = 0;
            b = ((y+dx)/2) + 30;
          } else {
            int source_idx = (source_line+dy) * display->pitch + (x+dx) * display->format->BytesPerPixel;
  
            b = ((((long)((Uint8 *)(display->pixels))[source_idx++]) * (340-y+dy)) >> 9) + ((y+dy)/2) + 30;
            g = (((long)((Uint8 *)(display->pixels))[source_idx++]) * (340-y+dy)) >> 9;
            r = (((long)((Uint8 *)(display->pixels))[source_idx++]) * (340-y+dy)) >> 9;
          }
  
          ((Uint8 *)(display->pixels))[targetidx++] = b;
          ((Uint8 *)(display->pixels))[targetidx++] = g;
          ((Uint8 *)(display->pixels))[targetidx++] = r;
  
          targetidx++;
        }
        source_line --;
      }
    } else {

      int horizontal_shift;
  
      for (int y = 0; y < (SCREENHEI / 2) - height; y++) {
  
        int target_line = (SCREENHEI / 2) + height + y;
        int source_line = (SCREENHEI / 2) + height - y - 1 - simple_waves[(wavetime * 4 + y * 2) & 0x7f];
        if (source_line < 0)
          source_line = 0;
  
        int z = simple_waves[(wavetime*5 + y) & 0x7f];
        if (abs(z - 4) > y) {
          if (z < 4)
            horizontal_shift = 4 - y;
          else
            horizontal_shift = 4 + y;
        } else {
          horizontal_shift = z;
        }
  
        {  // fill left and right edge with dark pixels
          int right_idx = target_line * display->pitch + (SCREENWID - 10) * display->format->BytesPerPixel;
          int left_idx = target_line * display->pitch;
          for (int x = 0; x < 10; x++) {
            ((Uint8 *)(display->pixels))[right_idx + 0] = 30 + y/2;
            ((Uint8 *)(display->pixels))[right_idx + 1] = 0;
            ((Uint8 *)(display->pixels))[right_idx + 2] = 0;
            ((Uint8 *)(display->pixels))[left_idx + 0] = 30 + y/2;
            ((Uint8 *)(display->pixels))[left_idx + 1] = 0;
            ((Uint8 *)(display->pixels))[left_idx + 2] = 0;
            right_idx += display->format->BytesPerPixel;
            left_idx += display->format->BytesPerPixel;
          }
        }
  
        {  // copy one pixel row
          int target_idx = target_line * display->pitch + horizontal_shift * display->format->BytesPerPixel;
          int source_idx = source_line * display->pitch;
          for (int x = 0; x < SCREENWID; x++) {
            if ((x + horizontal_shift > 0) && (x + horizontal_shift < SCREENWID)) {
              ((Uint8 *)(display->pixels))[target_idx + 0] = ((long)((Uint8 *)(display->pixels))[source_idx + 0]) * (340-y) / 512 + (y/2) + 30;
              ((Uint8 *)(display->pixels))[target_idx + 1] = ((long)((Uint8 *)(display->pixels))[source_idx + 1]) * (340-y) / 512;
              ((Uint8 *)(display->pixels))[target_idx + 2] = ((long)((Uint8 *)(display->pixels))[source_idx + 2]) * (340-y) / 512;
            }
            target_idx += display->format->BytesPerPixel;
            source_idx += display->format->BytesPerPixel;
          }
        }
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
      c = s[pos];
      if (fontchars[c].s != 0)
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

void scr_putbar(int x, int y, int br, int h, Uint8 colr, Uint8 colg, Uint8 colb, Uint8 alpha) {

  if (alpha != 255) {

    SDL_Surface *s = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, br, h, 24, 0xff, 0xff00, 0xff0000, 0);
    SDL_SetAlpha(s, SDL_SRCALPHA, alpha);

    SDL_Rect r;
    r.w = br;
    r.h = h;
    r.x = 0;
    r.y = 0;

    SDL_FillRect(s, &r, SDL_MapRGB(display->format, colr, colg, colb));

    scr_blit(s, x, y);

    SDL_FreeSurface(s);

  } else {

    SDL_Rect r;
    r.w = br;
    r.h = h;
    r.x = x;
    r.y = y;
    SDL_FillRect(display, &r, SDL_MapRGBA(display->format, colr, colg, colb, alpha));
  }
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
  if (key_keypressed(quit_action)) return;
  if (!tt_has_focus) {
      scr_darkenscreen();
      SDL_UpdateRect(display, 0, 0, 0, 0);
      wait_for_focus();
  }
  SDL_UpdateRect(display, 0, 0, 0, 0);
}

void scr_setclipping(int x, int y, int w, int h) {
    if (x < 0) SDL_SetClipRect(display, NULL);
    else {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	SDL_SetClipRect(display, &r);
    }
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

      if ((a > 72) || !doors[a].width)
        continue;

      if (lev_is_door(slice, col)) {
        if (lev_is_door_upperend(slice, col))
          scr_blit(spr_spritedata(doors[a].s[2]), (SCREENWID / 2) + doors[a].xstart, ypos);
        else if (lev_is_door_upperend(slice + 1, col))
          scr_blit(spr_spritedata(doors[a].s[1]), (SCREENWID / 2) + doors[a].xstart, ypos);
        else
          scr_blit(spr_spritedata(doors[a].s[0]), (SCREENWID / 2) + doors[a].xstart, ypos);
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

      if ((a > 72) || !doors[a].width)
        continue;

      if (lev_is_door(slice, col)) {
        if (lev_is_targetdoor(slice, col) && (state & 1)) continue;
        if (lev_is_door_upperend(slice, col))
          scr_blit(spr_spritedata(doors[a].s[2]), (SCREENWID / 2) + doors[a].xstart, ypos);
        else if (lev_is_door_upperend(slice + 1, col))
          scr_blit(spr_spritedata(doors[a].s[1]), (SCREENWID / 2) + doors[a].xstart, ypos);
        else
          scr_blit(spr_spritedata(doors[a].s[0]), (SCREENWID / 2) + doors[a].xstart, ypos);
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

  case TB_EMPTY:
    /* blank case */
    break;

  case TB_ELEV_BOTTOM:
  case TB_ELEV_TOP:
  case TB_ELEV_MIDDLE:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h);

    break;

  case TB_STEP:
  case TB_STEP_VANISHER:
  case TB_STEP_LSLIDER:
    scr_blit(spr_spritedata((angle % SPR_STEPFRAMES) + step), x - (SPR_STEPWID / 2), h);

    break;

  case TB_STICK:
  case TB_STICK_BOTTOM:
  case TB_STICK_MIDDLE:
    scr_blit(spr_spritedata(stick), x - (SPR_STICKWID / 2), h);

    break;

  case TB_BOX:
    scr_blit(spr_spritedata(boxst + boxstate), x - (SPR_BOXWID / 2), h);

    break;
  }
}

static void putcase_editor(unsigned char w, long x, long h, int state) {
  long angle = 0;
  switch (w) {

  case TB_EMPTY:
    /* blank case */
    break;

  case TB_ELEV_BOTTOM:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h - (state % 4));
    break;
  case TB_STATION_MIDDLE:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h - SPR_SLICEHEI/2 + abs(state - 8));
    break;
  case TB_STATION_TOP:
    scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), x - (SPR_ELEVAWID / 2), h + (state % 4));
    break;
  case TB_STEP:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2), h);
    break;
  case TB_STEP_VANISHER:
    if (use_alpha_sprites) {
	SDL_Surface *s = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, SPR_STEPWID, SPR_STEPHEI, 24, 0xff, 0xff00, 0xff0000, 0);
	SDL_Rect r;
	r.w = SPR_STEPWID;
	r.h = SPR_STEPHEI;
	r.x = 0;
	r.y = 0;
	SDL_BlitSurface(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), NULL, s, &r);
	SDL_SetAlpha(s, SDL_SRCALPHA, 96);
	scr_blit(s, x - (SPR_STEPWID / 2), h);
	SDL_FreeSurface(s);
    } else {
	if (state & 1)
	  scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2), h);
    }
    break;
  case TB_STEP_LSLIDER:
    scr_blit(spr_spritedata(((angle % SPR_STEPFRAMES) + step)), x - (SPR_STEPWID / 2) + state % 4, h);
    break;

  case TB_STICK:
    scr_blit(spr_spritedata(stick), x - (SPR_STICKWID / 2), h);
    break;

  case TB_BOX:
    scr_blit(spr_spritedata(boxst + boxstate), x - (SPR_BOXWID / 2), h);
    break;

  case TB_ROBOT1:
    scr_blit(spr_spritedata(ballst + 1), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2);
    break;
  case TB_ROBOT2:
    scr_blit(spr_spritedata(ballst), x - (SPR_ROBOTWID / 2) + state / 2, h - SPR_ROBOTHEI/2);
    break;
  case TB_ROBOT3:
    scr_blit(spr_spritedata(ballst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2);
    break;
  case TB_ROBOT4:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI/2 + abs(state - 8));
    break;
  case TB_ROBOT5:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2), h - SPR_ROBOTHEI + abs(state - 8) * 2);
    break;
  case TB_ROBOT6:
    scr_blit(spr_spritedata(robotsst), x - (SPR_ROBOTWID / 2) + abs(state - 8), h - SPR_SLICEHEI/2);
    break;
  case TB_ROBOT7:
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

void scr_writetext(long x, long y, const char *s, int maxchars) {
  int t = 0;
  unsigned char c;
  while (s[t] && (maxchars-- != 0)) {
    if (s[t] == ' ') {
      x += FONTMINWID;
      t++;
      continue;
    }

    c = s[t];
    if (fontchars[c].s != 0) {
      scr_blit(spr_spritedata(fontchars[c].s), x, y);
      x += fontchars[c].width + 3;
    }
    t++;
  }
}

void scr_writeformattext(long x, long y, const char *s) {

  int origx = x;
  int t = 0;
  Uint8 towerblock = 0;
  unsigned char c;
  while (s[t]) {
    switch(s[t]) {
    case ' ':
      x += FONTMINWID;
      t++;
      break;
    case '~':
      switch(s[t+1]) {
      case 't':
        x = (s[t+2] - '0') * 100 + (s[t+3] - '0') * 10 + (s[t+4] - '0');
        t += 5;
        break;
      case 'T':
	x = origx + (s[t+2] - '0') * 100 + (s[t+3] - '0') * 10 + (s[t+4] - '0');
	t += 5;
	break;
      case 'b':
	towerblock = conv_char2towercode(s[t+2]);
	putcase(towerblock, x+16, y);
	x += 32;
	t += 3;
	break;
      case 'e':
	towerblock = conv_char2towercode(s[t+2]);
	putcase_editor(towerblock, x+16, y, boxstate);
	x += 32;
	t += 3;
	break;
      default:
        assert(0, "Wrong command in formatted text.");
        t += 2;
      }
      break;
    default:
      c = s[t];
      if (fontchars[c].s != 0) {
        scr_blit(spr_spritedata(fontchars[c].s), x, y);
        x += fontchars[c].width + 3;
      }
      t++;
    }
  }
}

long scr_formattextlength(long x, long y, const char *s) {
  int origx = x;
  int t = 0;
  unsigned char c;
  while (s[t]) {
    switch(s[t]) {
    case ' ':
      x += FONTMINWID;
      t++;
      break;
    case '~':
      switch(s[t+1]) {
      case 't':
        x = (s[t+2] - '0') * 100 + (s[t+3] - '0') * 10 + (s[t+4] - '0');
        t += 5;
        break;
      case 'T':
	x = origx + (s[t+2] - '0') * 100 + (s[t+3] - '0') * 10 + (s[t+4] - '0');
	t += 5;
	break;
      case 'b':
	x += 32;
	t += 3;
	break;
      case 'e':
	x += 32;
	t += 3;
	break;
      default:
        assert(0, "Wrong command in formatted text, scr_formattextlength.");
        t += 2;
      }
      break;
    default:
      c = s[t];
      if (fontchars[c].s != 0) {
        x += fontchars[c].width + 3;
      }
      t++;
    }
  }
  return (x-origx);
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

    /* if the the current robot is active and not the cross */
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
static void draw_data(int time, screenflag flags)
{
  char s[256];
  int t;
  int y = status_top ? 5 : SCREENHEI - FONTHEI;

  if (time > 0) {
    sprintf(s, "%u", time);
    scr_writetext_center(y, s);
  }

  sprintf(s, "%u", pts_points());
  scr_writetext(5L, y, s);

  *s = '\0';
  if (pts_lifes() < 4)
    for (t = 0; t < pts_lifes(); t++)
      sprintf(s + strlen(s), "%c", fonttoppler);
  else sprintf(s, "%ix%c", pts_lifes(), fonttoppler);
  scr_writetext(SCREENWID - scr_textlength(s) - 5, y, s);

  y = status_top ? SCREENHEI - FONTHEI : 5;
  switch (flags) {
    case SF_REC:  if (!(boxstate & 8)) scr_writetext_center(y, "REC"); break;
    case SF_DEMO: scr_writetext_center(y, "DEMO"); break;
    case SF_NONE:
    default:      break;
  }
}

void scr_drawall(long vert,
                 long angle,
                 long time,
                 bool svisible,
                 int subshape,
                 int substart,
                 screenflag flags
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
                              ((top_look_left()) ?  mirror : 0)),
               (SCREENWID / 2) - (SPR_HEROWID / 2),
               (vert - top_verticalpos()) * 4 + (SCREENHEI / 2) - SPR_HEROHEI);

    if (top_onelevator())
      scr_blit(spr_spritedata((angle % SPR_ELEVAFRAMES) + elevatorsprite), (SCREENWID / 2) - (SPR_ELEVAWID / 2),
               vert - top_verticalpos() + (SCREENHEI / 2));
  }

  if (svisible) {
    /* TODO: use SPR_SUBMxxx */
    scr_blit(spr_spritedata(subst + subshape),
             (SCREENWID / 2) - 70,
             (SCREENHEI / 2) + 12 - substart + 16);

  }

  putkreuz(vert);

  putbattlement(angle, vert * 4);

  putwater(vert);

  draw_data(time, flags);

  boxstate = (boxstate + 1) & 0xf;
}

void scr_drawedit(long vpos, long apos, bool showtime) {

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
    scr_putrect((SCREENWID / 2) - (32 / 2), (SCREENHEI / 2) - 16, 32, 16, 
                boxstate * 0xf, boxstate *0xf, boxstate *0xf, 128);
  }

  if (showtime) {
      char s[20];
      sprintf(s, "%u", lev_towertime());
      scr_writetext_center(5, s);
  }

  boxstate = (boxstate + 1) & 0xf;
}

static void put_scrollerlayer(long horiz, int layer) {
  horiz %= scroll_layers[layer].width;
  scr_blit(spr_spritedata(scroll_layers[layer].image), -horiz, 0);
  if (horiz + SCREENWID > scroll_layers[layer].width)
    scr_blit(spr_spritedata(scroll_layers[layer].image),
             scroll_layers[layer].width - horiz, 0);
}

void scr_draw_bonus1(long horiz, long towerpos) {
  int l;

  for (l = 0; (l < num_scrolllayers) && (l < sl_tower_depth); l++)
    put_scrollerlayer(scroll_layers[l].num*horiz/scroll_layers[l].den, l);

  puttower(0, SCREENHEI/2, SCREENHEI, sl_tower_num*towerpos/sl_tower_den);
}

void scr_draw_bonus2(long horiz, long towerpos) {
  int l;
  
  for (l = sl_tower_depth; l < num_scrolllayers; l++) 
      put_scrollerlayer(scroll_layers[l].num*horiz/scroll_layers[l].den, l);

  draw_data(-1, SF_NONE);
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
