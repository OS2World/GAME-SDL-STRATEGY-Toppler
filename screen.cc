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

static int slicestart;
static int battlementstart;

static unsigned short robotsst, ballst, boxst, snowballst, starst, crossst,
         fishst, subst, haube, torb;
static int topplerstart;

unsigned short  step, elevatorsprite, stick;

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

struct {
  int xs;
  int br;
  unsigned short s[3];
} doors[73];

static SDL_Surface *fontchars[59];  //32..90

#define scrolllayers 3

static long layerwidth[scrolllayers];
static SDL_Surface *layerimage[scrolllayers];
char scrollerpalette[168*3];

/* this functions are so complex because the graphics are save in a
 way that allowed easy loading into the video memory in a tricky vga
 graphics mode in old times, now we need to sort the pixels in an
 orderly manner */
static void decomp(unsigned char *src, SDL_Surface *dst, int add, int count, bool sprite, bool descramble) {

  int pos = 0;

  int b = 0;

  /* error */
  if (count != dst->w*dst->h)
    return;

  int y = 0;
  int l = 0;
  int x = 0;
  int bp = 0;

  while (count > 0) {

    switch (bp) {
      case 0: b = (src[pos+1] >> 4) & 0xF; break;
      case 1: b = (src[pos+1] >> 0) & 0xF; break;
      case 2: b = (src[pos] >> 4) & 0xF; break;
      case 3: b = (src[pos] >> 0) & 0xF; break;
    }
    bp++;
    if (bp >= 4) {
      bp = 0;
      pos += 2;
    }

    if ((!sprite) || (b != 0))
        b = b + add - 1;

    if (descramble) {
      ((char *)(dst->pixels))[y*dst->pitch+x+l] = b;
  
      x += 4;
      if (x+l >= dst->w) {
        x = 0;
        y++;
        if (y >= dst->h) {
          y = 0;
          l++;
        }
      }
    } else {
      ((char *)(dst->pixels))[y*dst->pitch+x] = b;
      x += 1;
      if (x >= dst->w) {
        x = 0;
        y++;
        if (y >= dst->h) {
          y = 0;
        }
      }
    }

    count--;
  }
}

/* loads a sprite from the open file in arch, drscrambles it and saves it
 into the spritecollection returning the index */
unsigned short scr_loadsprites(int num, const int w, const int h, int colstart, bool sprite, bool descr) {
  unsigned short erg = 0;
  unsigned char p[w * h / 2];
  SDL_Surface *z;
  int res;

  for (int t = 0; t < num; t++) {
    arc_read(p, w * h / 2, &res);

    if (sprite)
      z = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, w, h, 8, 0, 0, 0, 0);
    else
      z = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8, 0, 0, 0, 0);

    pal_setstdpalette(z);

    if (sprite) SDL_SetColorKey(z, SDL_SRCCOLORKEY, 0);

    decomp(p, z, colstart, w * h, sprite, descr);

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

  for (int t = 0; t < cnt; t++) {
    r = arc_getbits(8);
    g = arc_getbits(8);
    b = arc_getbits(8);
    pal_setpal(colstart + t, r, g, b);
  }
}

/* another descramble routine for sprites */
static unsigned short loadsprites(int num, int w, int h, int bits, int colstart) {
  unsigned char b;
  unsigned short erg = 0;
  SDL_Surface *z;

  for (int t = 0; t < num; t++) {
    z = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                             w, h, 8, 0, 0, 0, 0);
    pal_setstdpalette(z);
    SDL_SetColorKey(z, SDL_SRCCOLORKEY, 0);

    for (int l = 0; l < 4; l++)
      for (int y = 0; y < h; y++)
        for (int x = 0; x < (w / 4); x ++) {
          b = arc_getbits(bits);
          if (b != 0) b += colstart;
          ((char *)(z->pixels))[y*w+4*x+l] = b;
        }

    if (t == 0)
      erg = spr_savesprite(z);
    else
      spr_savesprite(z);
  }

  return erg;
}

/* loads all the graphics */
static void loadgraphics() {

  int res;
  unsigned char pal[192];

  arc_assign(grafdat);

  for (int t = 0; t < brickcnt; t++) {
    unsigned char c1, c2;

    arc_read(&c1, 1, &res);
    arc_read(&c2, 1, &res);

    pal_setbrickpal(t, c1, c2);
  }

  for (int t = 0; t < 8; t++)
    pal_setpal(topplercol + t, pal[t * 3], pal[t * 3 + 1], pal[t * 3 + 2]);

  slicestart = scr_loadsprites(8, 96, 8, brickcol, false, false);
  battlementstart = scr_loadsprites(8, 144, 24, brickcol, false, false);

  for (int t = -36; t <= 36; t++) {
    arc_read(&doors[t+36].xs, 2, &res);
    arc_read(&doors[t+36].br, 2, &res);

    for (int et = 0; et < 3; et++)

      if (doors[t+36].br != 0)
        doors[t+36].s[et] = scr_loadsprites(1, doors[t+36].br, 8, brickcol, false, true);
      else
        doors[t+36].s[et] = 0;
  }

  arc_read(&pal[0], envirocnt*2, &res);
  for (int t = 0; t < envirocnt; t++)
    pal_setpal(envirocol + t, pal[2*t+1], pal[2*t], pal[2*t]);

  step = scr_loadsprites(1, 20, 7, envirocol, false, true);
  elevatorsprite = scr_loadsprites(1, 16, 7, envirocol, false, true);
  stick = scr_loadsprites(1, 8, 7, envirocol, false, true);

  arc_closefile();

  arc_assign(topplerdat);

  arc_read(pal, 8*3, &res);
  for (int t = 0; t < 8; t++)
    pal_setpal(topplercol + t, pal[t * 3], pal[t * 3 + 1], pal[t * 3 + 2]);

  topplerstart = scr_loadsprites(74, 20, 20, topplercol, true, true);

  arc_assign(spritedat);

  loadcolors(robotscnt, robotscol);
  robotsst = loadsprites(128, 16, 16, 6, robotscol);

  loadcolors(ballcnt, ballcol);
  ballst = loadsprites(2, 16, 16, 5, ballcol);

  loadcolors(boxcnt, boxcol);
  boxst = loadsprites(16, 8, 8, 4, boxcol);

  loadcolors(snowballcnt, snowballcol);
  snowballst = loadsprites(1, 8, 8, 3, snowballcol);

  loadcolors(starcnt, starcol);
  starst = loadsprites(16, 16, 16, 3, starcol);
  sts_init(starst + 10);

  loadcolors(subcnt, subcol);
  fishst = loadsprites(16, 20, 20, 6, subcol);
  haube = loadsprites(1, 20, 20, 6, subcol);
  subst = loadsprites(6, 60, 17, 6, subcol);
  torb = loadsprites(1, 16, 16, 6, subcol);

  arc_closefile();

  arc_assign(crossdat);

  for (int t = 0; t < crosscnt; t++) {
    int r = arc_getbits(8);
    int g = arc_getbits(8);
    int b = arc_getbits(8);

    pal_setcrosspal(t, r, g, b);
  }

  crossst = loadsprites(120, 16, 16, 4, crosscol);

  arc_closefile();
}

static void loadfont() {

  unsigned char pal[fontcnt*3];
  int res;
  unsigned char p[6*16];
  SDL_Surface *s;
  char c;

  arc_assign(fontdat);

  arc_read(pal, fontcnt*3, &res);

  for (int t = 0; t < fontcnt-1; t++)
    pal_setpal(t+fontcol, pal[3*t], pal[3*t+1], pal[3*t+2]);

  while (!arc_eof()) {
    s = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                               12, 16, 8, 0, 0, 0, 0);
    SDL_SetColorKey(s, SDL_SRCCOLORKEY, 0);
    pal_setstdpalette(s);
    arc_read(&c, 1, &res);
    arc_read(p, 16*6, &res);
    decomp(p, s, fontcol, 16*12, true, true);
    fontchars[c-32] = s;
  }

  arc_closefile();
}

static void loadscroller() {
  FILE *in = file_open("layer1.tga", "rb");
  layerimage[0] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                               640, 240, 8, 0, 0, 0, 0);
    SDL_SetColorKey(layerimage[0], SDL_SRCCOLORKEY, 0);
    pal_setstdpalette(layerimage[0]);
  fread((layerimage[0]->pixels), 500, 1, in);
  fread((layerimage[0]->pixels), 640*240, 1, in);
  fclose(in);
  
  in = file_open("layer2.tga", "rb");
  layerimage[1] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                               640, 240, 8, 0, 0, 0, 0);
    SDL_SetColorKey(layerimage[1], SDL_SRCCOLORKEY, 0);
    pal_setstdpalette(layerimage[1]);
  fread((layerimage[1]->pixels), 18, 1, in);
  fread(scrollerpalette, 152*3, 1, in);
  fread((layerimage[1]->pixels), 26, 1, in);
  
  fread((layerimage[1]->pixels), 640*240, 1, in);
  fclose(in);
  
  in = file_open("layer3.tga", "rb");
  layerimage[2] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY,
                               640, 240, 8, 0, 0, 0, 0);
    SDL_SetColorKey(layerimage[2], SDL_SRCCOLORKEY, 0);
    pal_setstdpalette(layerimage[2]);
  fread((layerimage[2]->pixels), 500, 1, in);
  fread((layerimage[2]->pixels), 640*240, 1, in);
  fclose(in);
  
  for (int i = 0; i < 640*240; i++) {
    if (((char *)(layerimage[0]->pixels))[i])
      ((char *)(layerimage[0]->pixels))[i] += 16;
    if (((char *)(layerimage[1]->pixels))[i])
      ((char *)(layerimage[1]->pixels))[i] += 16;
    if (((char *)(layerimage[2]->pixels))[i])
      ((char *)(layerimage[2]->pixels))[i] += 16;
  }

  layerwidth[0] = layerwidth[1] = layerwidth[2] = 640;
  
}

void scr_init() {
  spr_init(600);  /* 597 is the number of sprites, just a few for savety */
  loadgraphics();
  loadfont();
  loadscroller();
  display = SDL_SetVideoMode(320*2, 240*2, 8,
                             SDL_HWPALETTE | ((fullscreen) ? (SDL_FULLSCREEN) : (0)));
  second = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 8, 0, 0, 0, 0);
  pal_setstdpalette(second);
}

void scr_done() {
  SDL_FreeSurface(second);
  spr_done();
}

/*Malaktionen beziehen sich immer auf inaktve Seite */


static void cleardesk(long height) {
  SDL_Rect r;
  r.w = 320;
  r.h = 240;
  r.x = r.y = 0;
  SDL_FillRect(second, &r, 0);
}

static void puttower(long angle, long height, long towerheight, int shift = 0) {

  int w, upend, a;

  height *= 2;

  if (height < 120)
    w = 112 + height;
  else
    w = 232 + (height & 0x7);

  upend = 120 - (towerheight - height);

  a = ((height - w) / 2) & 4;

  if (w > 232)
    scr_blit(spr_spritedata((angle + a) & 0x7 + slicestart), 112 + shift, w);

  while ((w >= 0) && (w >= upend)) {
    scr_blit(spr_spritedata((angle + a) & 0x7 + slicestart), 112 + shift, w);
    w -= 8;
    a += 4;
  }

  if ((w < 0) && (w>-8) && (w>=upend))
    scr_blit(spr_spritedata((angle+a) & 0x7 + slicestart), 112, w);
}

static void putbattlement(long angle, long height) {

  int upend;

  height *= 2;

  upend = 120-(lev_towerheight()-height);

  if (upend > 0)
    scr_blit(spr_spritedata((angle & 7) + battlementstart), 88, upend - 24);
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

  if (height < 120) {
    for (y = 1; y <= 120 - height; y++) {
      t = 120 + height - y - 1 - waves[(wavetime * 4 + y * 2) & 0x7f];
      if (t <= 0) t = 0;
      z = waves[(wavetime*5 + y) & 0x7f];
      if (abs(z - 4) > y) {
        if (z < 4) {
          v = 4-y;
          z = 120 + height + y - 1;
        } else {
          v = 4+y;
          z =120 + height + y - 1;
        }
      } else {
        v = z;
        z = 120 + height + y - 1;
      }

      for(x = 0; x < 10; x++) {
        ((unsigned char *)(second->pixels))[z*320+x] = pal_dark(0);
        ((unsigned char *)(second->pixels))[z*320+x+310] = pal_dark(0);
      }

      for(x = 0; x < 320; x++) {
        if ((x+v > 0) && (x+v < 320))
          ((unsigned char *)(second->pixels))[z*320+x+v] =
            pal_dark(((unsigned char *)(second->pixels))[t*320+x]);
      }
    }
  }
  wavetime++;
}

/*Schreibt Text mit aktuellem Font*/
void scr_writetext(long x, long y, char *s) {
  int t = 0;
  unsigned char c;
  while (s[t] != 0) {
    if ((s[t] >= 'a') && (s[t] <= 'z'))
      c = s[t] - 'a' + 'A' -32;
    else
      c = s[t] - 32;
    if ((c < 59) && (fontchars[c] != NULL))
      scr_blit(fontchars[c], x, y);
    x += 12;
    t++;
  }
}

void scr_putbar(int x, int y, int br, int h) {
  SDL_Rect r;
  r.w = br;
  r.h = h;
  r.x = x;
  r.y = y;
  SDL_FillRect(second, &r, 0);
}

/* exchange active and inactive page */
void scr_swap() {
  int p = 0;
  int q = 0;
  unsigned char i = ((unsigned char *)(second->pixels))[0];
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      ((char *)(display->pixels))[p++] = i;
      ((char *)(display->pixels))[p++] = i;
      q++;
      i = ((unsigned char *)(second->pixels))[q];
    }
    memmove(&((char *)(display->pixels))[p], &((char *)(display->pixels))[p - 640], 640);
    p += 640;
  }
/*
  int p = 0;
  int q = 0;
  unsigned char i = ((unsigned char *)(second->pixels))[0];
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 320; x++) {
      ((char *)(display->pixels))[p++] = pal_blue(i);
      ((char *)(display->pixels))[p++] = pal_green(i);
      ((char *)(display->pixels))[p++] = pal_red(i);
      ((char *)(display->pixels))[p++] = ((char *)(display->pixels))[p-3];
      ((char *)(display->pixels))[p++] = ((char *)(display->pixels))[p-3];
      ((char *)(display->pixels))[p++] = ((char *)(display->pixels))[p-3];
      q++;
      i = ((unsigned char *)(second->pixels))[q];
    }
    memmove(&((char *)(display->pixels))[p], &((char *)(display->pixels))[p - 640*3], 640*3);
    p += 640*3;
  }*/
/*
  pal_setpalette(second);
  SDL_BlitSurface(second, NULL, display, NULL);
  pal_setstdpalette(second);*/
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

  puttower(angle, vert, lev_towerheight());

  a = -36;
  while (((a - angle) & 0x7) != 0)
    a++;

  while (a <= 36) {
    an = ((a - angle) / 8) & 0xf;
    y = (vert * 2) - (hs * 8) + 112;
    for (h = hs; h <= he; h++) {
      if (lev_is_door_upperend(h, an) && doors[a + 36].br) {
        scr_blit(spr_spritedata(doors[a + 36].s[2]), doors[a + 36].xs, y);
        scr_blit(spr_spritedata(doors[a + 36].s[1]), doors[a + 36].xs, y-8);
        scr_blit(spr_spritedata(doors[a + 36].s[0]), doors[a + 36].xs, y-16);
      }
      y -= 8;
    }
    a += 8;
  }
}

/* draws something of the environment */
static void putcase(unsigned char w, long x, long h) {
  switch (w) {

  case 0:
    /* blank case */
    break;

  case 0x85:
  case 0x89:
  case 0x8d:
    scr_blit(spr_spritedata(elevatorsprite), 152 + x, h);

    break;

  case 0x81:
  case 0x91:
  case 0xb1:
    scr_blit(spr_spritedata(step), 150 + x, h);

    break;

  case 0x80:
  case 0x84:
  case 0x8c:
    scr_blit(spr_spritedata(stick), 156 + x, h);

    break;

  case 0x82:
    scr_blit(spr_spritedata(boxst + boxstate), x + 160 - 4, h);

    break;
  }
}


/* draws a robot */
static void putrobot(int t, int m, long x, long h)
{
  int nr;

  if (h > 256) return;

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
      nr = robotsst + lev_towernr() * 16 + ((m / 2) & 0xf);
      break;
  
    default:
      nr = 40;
      break;
  }

  scr_blit(spr_spritedata(nr), x + 160 - 8, h - 16);
}

/* draws something of the tower */
static void putthings(long vert, long a, long angle, long hs, long he) {

  long x, y, h;

  x = sintab[a + 60];
  y = (vert * 2) - (hs * 8) + 112;
  for (h = hs; h <= he; h++) {
    putcase(lev_tower(h, ((a - angle) / 8) & 0xf), x, y);
    y -= 8;
  }

  for (h = 0; h <= 3; h++) {
    if (rob_kind(h) != OBJ_KIND_NOTHING && rob_kind(h) != OBJ_KIND_CROSS) {
      x = ((rob_angle(h) + angle + 56) & 0x7f) - 52;
      if (x > a && x <= a + 8)
        putrobot(rob_kind(h), rob_time(h),
                 sintab[x + 52], ((vert - rob_vertical(h)) * 2) + 120);
    }
  }
}

/* draws everything behind the tower */
static void draw_behind(long vert, long angle, long hs, long he)
{
  long a;

  a = -60;
  while ((a - angle) & 0x7) a++;

  while (a <= -37) {
    putthings(vert, a, angle, hs, he);
    a += 8;
  }

  a = 60;
  while ((a - angle) & 0x7) a--;

  while (a >= 37) {
    putthings(vert, a, angle, hs, he);
    a -= 8;
  }
}


/* draws everything in front of the tower */
static void draw_bevore(long  vert, long angle, long hs, long he)
{
  long a;

  a = -36;
  while ((a - angle) & 0x7) a++;

  while (a <= -1) {
    putthings(vert, a, angle, hs, he);
    a += 8;
  }

  a = 36;
  while ((a - angle) & 0x7) a--;

  while (a >= 0) {
    putthings(vert, a, angle, hs, he);
    a -= 8;
  }
}

/* draws the cross the moves to and fro over the screen */
static void putkreuz(long vert)
{
  long i, y;

  for (int t = 0; t <= 3; t++) {
    if (rob_kind(t) == OBJ_KIND_CROSS) {
      i = rob_angle(t) * 3 - 28;
      if ((i <= -16) || (i >= 320))
        return;
      y = (vert - rob_vertical(t)) * 2 + 104;
      if (y > -16 && y < 240)
        scr_blit(spr_spritedata(crossst + labs(rob_time(t)) % 120), i, y);

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
    scr_writetext(160 - strlen(s) * 6L, 5L, s);
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

  cleardesk(vert);

  hs = vert / 4 - 16;
  if (hs < 0) hs = 0;

  he = vert / 4 + 16;
  if (he * 8 > lev_towerheight()) he = (lev_towerheight() / 8) - 1;

  sts_draw();
  draw_behind(vert, angle, hs, he);
  draw_tower(vert, angle, hs, he);
  draw_bevore(vert, angle, hs, he);

  if (snb_exists())
    scr_blit(spr_spritedata(snowballst),
             sintab[((snb_anglepos() + angle) & 0x7f) + 60] + 148,
             ((vert - snb_verticalpos()) * 2) + 112);

  if (top_visible()) {
      scr_blit(spr_spritedata(topplerstart + top_shape() +
                              ((top_look_left()) ?  umkehr : 0)),
               150, (vert - top_verticalpos()) * 2 + 100);

    if (top_onelevator())
      scr_blit(spr_spritedata(elevatorsprite), 152, vert - top_verticalpos() + 120);

  }

  if (svisible) {
    scr_blit(spr_spritedata((long)subst),
             115 + 16,
             132 - substart + 16);

    scr_blit(spr_spritedata(haube),
             171 - subshape - 16 - 8,
             120 - substart + 8 * 2);

  }

  putkreuz(vert);

  putbattlement(angle, vert);

  putwater(vert);

  draw_data(time);

  boxstate = (boxstate + 1) & 0xf;
}

static void put_scrollerlayer(long horiz, int layer) {
  horiz %= layerwidth[layer];
  scr_blit(layerimage[layer], -horiz, 0);
  if (horiz + 320 > layerwidth[layer])
    scr_blit(layerimage[layer], layerwidth[layer] - horiz, 0);
}

void scr_draw_bonus1(long horiz, long towerpos) {
  put_scrollerlayer(horiz/2, 0);
  put_scrollerlayer(horiz  , 1);

  puttower(1, 60, 240, towerpos);
}
void scr_draw_bonus2(long horiz, long towerpos) {
  put_scrollerlayer(horiz*2, 2);

  draw_data(-1);
}

void scr_draw_submarine(long vert, long x, long number) {
  scr_blit(spr_spritedata(subst+number), x, vert);
  scr_blit(spr_spritedata(haube), x+20, vert-12);
}

void scr_draw_fish(long vert, long x, long number) {
  scr_blit(spr_spritedata(fishst+number), x, vert);
}

void scr_draw_torpedo(long vert, long x) {
  scr_blit(spr_spritedata(torb), x, vert);
}
