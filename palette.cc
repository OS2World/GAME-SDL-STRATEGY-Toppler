#include "palette.h"

#include "decl.h"

#include <stdlib.h>

typedef struct {
  unsigned char r, g, b;
} color;

extern SDL_Surface *display;

static color pal[256];
static color crosspal[crosscnt];
static color towercolors[brickcnt];
static unsigned char dark[256];

void pal_setpal(unsigned char nr, unsigned char r, unsigned char g, unsigned char b) {
  pal[nr].r = r;
  pal[nr].g = g;
  pal[nr].b = b;
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

    pal_setpal(t+brickcol, rw, gw, bw);

    rw *= 8;
    gw *= 8;
    bw *= 8;

    rw /= 12;
    gw /= 12;
    bw /= 12;

    bw += 30;

    pal_setpal(t+shadowcol, rw, gw, bw);
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

    pal[t + crosscol].r = r;
    pal[t + crosscol].g = g;
    pal[t + crosscol].b = b;

    r *= 2;
    g *= 2;
    b *= 2;

    r /= 3;
    g /= 3;
    b /= 3;

    b += 30;

    pal[t + brickcnt + shadowcol].r = r;
    pal[t + brickcnt + shadowcol].g = g;
    pal[t + brickcnt + shadowcol].b = b;
  }

  pal_calcdark();
  pal_colors();
}

void pal_colors()
{
  SDL_Color p[256];
  for (int i = 0; i < 256; i++) {
    p[i].r = pal[i].r;
    p[i].g = pal[i].g;
    p[i].b = pal[i].b;
  }

  SDL_SetColors(display, p, 0, 256);
}


void pal_black()
{
  SDL_Color p[256];

  for (int i = 0; i < 256; i++) {
    p[i].r = 0;
    p[i].g = 0;
    p[i].b = 0;
  }
  SDL_SetColors(display, p, 0, 256);
}


void pal_fade_in()
{
  int i;
  SDL_Color c[256];

  for (int f = 0; f < 256; f++) {
    for (i = 0; i < 256; i++) {
      c[i].r = (pal[i].r * f) >> 8;
      c[i].g = (pal[i].g * f) >> 8;
      c[i].b = (pal[i].b * f) >> 8;
    }

    SDL_SetColors(display, c, i, 256);
  }
}


void pal_fade_out()
{
}


void pal_darkening(int as, int ae) {
  SDL_Color p[256];

  for(int i = 0; i < 256; i++) {
    if ((i < as) || (i > ae)) {
      p[i].r = pal[i].r >> 1;
      p[i].g = pal[i].g >> 1;
      p[i].b = pal[i].b >> 1;
    } else {
      p[i].r = pal[i].r;
      p[i].g = pal[i].g;
      p[i].b = pal[i].b;
    }
  }
  SDL_SetColors(display, p, 0, 256);
}

void pal_savepal(void **p) {
  color *q = new color [256];

  *p = q;

  for (int i = 0; i < 256; i++) {
    q[i].r = pal[i].r;
    q[i].g = pal[i].g;
    q[i].b = pal[i].b;
  }
}


void pal_restorepal(void *p) {
  color *q = (color *)p;

  for (int i = 0; i < 256; i++) {
    pal[i].r = q[i].r;
    pal[i].g = q[i].g;
    pal[i].b = q[i].b;
  }

  delete [] q;
}

void pal_setpalette(SDL_Surface *s) {
  SDL_Color q[256];

  for (int i = 0; i < 256; i++) {
    q[i].r = pal[i].r;
    q[i].g = pal[i].g;
    q[i].b = pal[i].b;
  }

  SDL_SetColors(s, q, 0, 256);
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

void pal_calcdark() {
  int r, g, b, m;

  for (int t = 0; t < 256; t++) {
    r = pal[t].r;
    g = pal[t].g;
    b = pal[t].b;

    r *= 8;
    g *= 8;
    b *= 8;

    r /= 12;
    g /= 12;
    b /= 12;

    b += 30;

    m = 0;
    for (int z = 1; z < 256; z++)
      if ((abs(r-pal[z].r) + abs(b-pal[z].b) + abs(g-pal[z].g)) <
          (abs(r-pal[m].r) + abs(b-pal[m].b) + abs(g-pal[m].g)))
        m = z;

    dark[t] = m;
  }
}

unsigned char pal_dark(unsigned char p) { return dark[p]; }

char pal_red(unsigned char i) {
  return pal[i].r;
}

char pal_green(unsigned char i) {
  return pal[i].g;
}

char pal_blue(unsigned char i) {
  return pal[i].b;
}


