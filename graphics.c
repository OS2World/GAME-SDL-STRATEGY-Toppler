#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include "colorreduction.h"
#include "pngsaver.h"

#define radius          96
#define steinzahl       16
#define hoehe           16

#define wadd            (2 * M_PI / steinzahl)

#define steprad         20
#define elevatorrad     16
#define stickrad        7

#define zinnenrad       (radius + 48)

SDL_Surface *display;
SDL_Surface *brick, *zinne;

double lw = 10 * M_PI / 180;

double asin(double x)
{
  if (x == 1)
    return (M_PI / 2);
  else {
    if (x == -1)
      return (M_PI / -2);
    else
      return atan(x / sqrt(1 - x * x));
  }
}

void putpixel(unsigned short x, unsigned short y, Uint32 b)
{
  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+2] = b;
  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+1] = b >> 8;
  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+0] = b >> 16;
}

void putpixel_max(unsigned short x, unsigned short y, Uint32 t)
{
  Uint16 r, g, b, rp, gp, bp;

  r = ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+2];
  g = ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+1];
  b = ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+0];

  rp = (t >>  0) & 0xff;
  gp = (t >>  8) & 0xff;
  bp = (t >> 16) & 0xff;

  if ((r != 0) || (g != 0) || (b != 0xff)) {
    rp = (rp + r) / 2;
    gp = (gp + g) / 2;
    bp = (bp + b) / 2;
  }

  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+2] = rp;
  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+1] = gp;
  ((Uint8*)display->pixels)[y*display->pitch+x*display->format->BytesPerPixel+0] = bp;
}

Uint8 getpixel(Uint16 x, Uint16 y)
{
   return ((Uint8*)display->pixels)[y * display->pitch + x * display->format->BytesPerPixel];
}

double fmax(double a, double b)
{
  if (a > b)
    return a;
  else
    return b;
}

void createdoor(double w, unsigned short ys, unsigned short xm, double doorwidth, double door_rad, double height, double doorl)
{
  int xl, xr, y, a;
  double nl, nr;
  double ll, lr;
  double x;
  Uint32 b;

  double doorlength = sqrt(4*door_rad*door_rad-doorwidth*doorwidth);

  if (doorl <= 0)
    doorl = doorlength;

  double doordiag = door_rad;
  double beta = atan2(doorwidth, doorlength);


  while (w >= M_PI)
    w -= 2 * M_PI;
  while (w < -M_PI)
    w += 2 * M_PI;

  /* ok, lets first calculate the x coordinates of the borders of the door */
  xl = (int)(door_rad * sin(w - beta) + 0.5);
  xr = (int)(door_rad * sin(w + beta) + 0.5);

  /* the door is invisible if the left end is right of the right end */
  if (xl > xr)
    return;

  /* now to the light calculations */

  /* these are the angles the light hits the left and right wall of the
   doorway */
  nl = fmax(0.2, cos(-M_PI/2 - lw - w));
  nr = fmax(0.2, cos(M_PI/2 - lw - w));

  /* and not how far the light reached into the tunnel bevore
   the other edge casts a shadow on it */
  {
    /* we do this by looking how far we can look into the tunnel at the
     opposide edge */
    double dxl = door_rad * sin (w - beta + lw);
    double dxr = door_rad * sin (w + beta + lw);

    ll = ((double)dxl-xr) / ((double)xr+dxl) * doorlength;
    lr = ((double)dxr-xl) / ((double)xr+dxl) * doorlength;
  }

  /* so, now lets draw the doors line by line from
   top to bottom, the size is fixed: three layers */
  for (y = 0; y < height; y++) {

    /* calculate the layer and the hight inside the layer */
    Uint8 layer = y / hoehe;
    Uint8 ystone = y % hoehe;

    /* draw from left end to the right end */
    for (x = xl; x < xr; x++) {

      /* we do anti aliasing so we need to sum up the colors */
      double c1, c2, c3;

      c1 = c2 = c3 = 0;

      /* eight times antialiasing */
      for (a = 0; a < 8; a++) {

        /* calc the atual position */
        double xpos = x + (double)a/8;

        /* if xpos < -xr then we are drawing part of the left wall, if xpos > -xl then
         we draw part of the right wall, in the last case we draw nothing because
         we look through the tunnel */
        if (xpos < -xr) {

          /* both cases are more or less the same, so I comment only one */

          /* first calculate what position of the wall we see at the current x-pos
           this is a simple linear equation that is 0 at xl (front edge) and doorlength
           at -xr (back edge of the tunnesl wall */
          double pixelpos = ((double)xl-xpos) / ((double)xr+xl) * doorlength;

          /* for rounding */
          pixelpos += 0.5;

          /* now lets calculate the position on the stone we see with the wall position given */
          double stonepos = pixelpos;

          /* at the middle layer it's shifted by half a stone width */
          if (layer == 1)
            stonepos += brick->w/2;

          while (stonepos >= brick->w)
            stonepos -= brick->w;

          /* get the color from the brick */
          b = *((Uint32 *)(((Uint8*)brick->pixels) + ystone * brick->pitch + (int)stonepos * brick->format->BytesPerPixel));

          /* calculate the light itensity using smooth shadow that is exactly on the
           middle way between dark and the max light at the sharow start postition we
           have calculated above */
          double n = (1-(pixelpos - (ll-10)) / (2*10)) * (nl -0.2) + 0.2;
          if (n < 0.2) n = 0.2;
          if (n > nl) n = nl;
          if (pixelpos > doorl) n = 0;

          /* add shadowed values */
          c1 += (double)(b & 0xff) * nl;
          c2 += (double)((b >> 8) & 0xff) * nl;
          c3 += (double)((b >> 16) & 0xff) * nl;

        } else if (xpos > -xl) {
          double pixelpos = ((double)xr-xpos) / ((double)xr+xl) * doorlength;
  
          pixelpos += 0.5;

          double stonepos = pixelpos;
  
          if (layer == 1)
            stonepos += brick->w/2;
  
          while (stonepos >= brick->w)
            stonepos -= brick->w;
  
          b = *((Uint32 *)(((Uint8*)brick->pixels) + ystone * brick->pitch + (int)stonepos * brick->format->BytesPerPixel));

          double n = (1-(pixelpos - (lr-5)) / (2*5)) * (nr -0.2) + 0.2;
          if (n < 0.2) n = 0.2;
          if (n > nr) n = nr;
          if (pixelpos > doorl) n = 0;

          c1 += (double)(b & 0xff) * n;
          c2 += (double)((b >> 8) & 0xff) * n;
          c3 += (double)((b >> 16) & 0xff) * n;
        }

      }

      /* calc average of the 8 samples */
      c1 /= 8;
      c2 /= 8;
      c3 /= 8;

      /* draw the pixel */
      b = (int)c3;
      b = (b << 8) | (int)c2;
      b = (b << 8) | (int)c1;
      putpixel((int)x+xm, y + ys, b);
    }
  }

  /* finally draw a black border around the door */
  /*
  for (y = ys; y < ys + hoehe * 3; y++) {
    putpixel(xl+xm, y, 0);
    putpixel(xr+xm, y, 0);
  }
  for (x = xl; x < xr; x++) {
    putpixel(x+xm, ys, 0);
    putpixel(x+xm, ys + hoehe*3-1, 0);
    }
    */
}


void putstein_slice(unsigned short ys, unsigned short x1, unsigned short x2, double n) {
  unsigned short x, y;
  long m;
  Uint32 b, c1, c2, c3, a;

  n = -n;

  if (n < 0.2) n = 0.2;

  for (y = 0; y < hoehe; y++) {
    for (x = x1; x < x2; x++) {


      c1 = c2 = c3 = 0;

      for (a = 0; a < 8; a++) {
        m = (long)((double)(x + (float)a / 8 - x1) / (x2 - x1) * brick->w + 0.5);
        if (m >= brick->w)
          m = brick->w - 1;

        b = *((Uint32 *)(((Uint8*)brick->pixels) + y * brick->pitch + m * brick->format->BytesPerPixel));

        c1 += b & 0xff;
        c2 += (b >> 8) & 0xff;
        c3 += (b >> 16) & 0xff;
      }

      c1 /= 8;
      c2 /= 8;
      c3 /= 8;

      c1 = (long)(c1 * n + 0.5);
      c2 = (long)(c2 * n + 0.5);
      c3 = (long)(c3 * n + 0.5);

      /*
       if (c1 < 0)
        c1 = 0;
      if (c2 < 0)
        c2 = 0;
      if (c3 < 0)
        c3 = 0;*/

      b = c3;
      b = (b << 8) | c2;
      b = (b << 8) | c1;

      putpixel(x, y + ys, b);
    }
  }
}

void createslice(double w, unsigned short ys) {
  double e, n;
  int t;

  for (t = 0; t < steinzahl; t++) {
    e = 2 * M_PI * t / steinzahl + w;
    int x1 = (int)(radius * cos(e) + radius + 0.5);
    int x2 = (int)(radius * cos(e + wadd) + radius + 0.5);
    n = sin(e + wadd + lw);
    if (x1 < x2)
      putstein_slice(ys, x1, x2, n);
  }
}

void createcyl(Uint16 r, FILE *out)
{
  int x, y, h;

  for (y = 0; y < hoehe - 1; y++) {
    for (x = -r; x <= r-1; x++) {
      h = (int)(255 * (1-fabs(sin(asin((double)x / r) + lw))) + 0.5);
      Uint8 c = h;

      fwrite(&c, 1, 1, out);
    }
  }
}


void putstein_pinacle(unsigned short ys, unsigned short x1, unsigned short x2, double n)
{
  unsigned short x, y;
  long m;
  Uint32 b, c1, c2, c3, a;

  n = -n;

  if (n < 0.2) n = 0.2;

  for (y = 0; y < hoehe * 3; y++) {
    for (x = x1; x < x2; x++) {


      c1 = c2 = c3 = 0;

      for (a = 0; a < 8; a++) {
        m = (long)((double)(x + (float)a / 8 - x1) / (x2 - x1) * zinne->w + 0.5);
        if (m >= zinne->w)
          m = zinne->w - 1;

        b = *((Uint32 *)(((Uint8*)zinne->pixels) + y * zinne->pitch + m * zinne->format->BytesPerPixel));

        c1 += b & 0xff;
        c2 += (b >> 8) & 0xff;
        c3 += (b >> 16) & 0xff;
      }

      c1 /= 8;
      c2 /= 8;
      c3 /= 8;

      c1 = (long)(c1 * n + 0.5);
      c2 = (long)(c2 * n + 0.5);
      c3 = (long)(c3 * n + 0.5);

      /*if (c1 < 0)
        c1 = 0;
      if (c2 < 0)
        c2 = 0;
      if (c3 < 0)
        c3 = 0;*/

      b = c3;
      b = (b << 8) | c2;
      b = (b << 8) | c1;

      if (b)
        putpixel_max(x, y + ys, b);
    }
  }
}

void writebrickpixel(Uint16 x, Uint16 y, FILE *out) {

  Uint8 c = getpixel(x, y);

  fwrite(&c, 1, 1, out);
}


void createzinne(unsigned short ys, double w)
{
  double e, n;
  long t, z, x1, x2, xl;
  long xmin = zinnenrad, xmax = zinnenrad;


  for (t = 0; t < steinzahl; t++) {
    e = 2 * M_PI * t / steinzahl + w;
    if ((e < M_PI/2) || (e > 3*M_PI/2))
      createdoor(e, ys, zinnenrad, 30, zinnenrad, 21, 10);
  }
  for (t = 0; t < steinzahl; t++) {
    e = 2 * M_PI * t / steinzahl + w;
    x1 = (long)floor(zinnenrad * cos(e) + zinnenrad + 0.5);
    x2 = (long)floor(zinnenrad * cos(e + wadd) + zinnenrad + 0.5);
    n = sin(e + wadd + lw);
    if (x1 < x2) {
      putstein_pinacle(ys, (int)x1, (int)x2, n);
      xl = x2;
      if (x1 < xmin) xmin = x1;
      if (x2 > xmax) xmax = x2;
    }
  }
  for (z = 0; z < 3 * 16; z++)
    for (t = 0; t < 2*zinnenrad; t++)
      if ((t < xmin) || (t >= xmax))
        putpixel(t, ys+z, SDL_MapRGBA(display->format, 0,255,0,255));

}

void getdoor(Uint16 ys, FILE *out) {

  int xs = -1;
  int br = 1;
  int x, y, t;

  Uint8 key = getpixel(display->w-1, 0);

  for (x = 0; x < 2*radius + 10; x++) {
    if (getpixel(x, ys) != key) {
      if (xs == -1)
        xs = x;
      else
        br++;
    }
  }

  if (xs == -1) {
    xs = 0;
    fwrite(&xs, 1, 2, out);
    fwrite(&xs, 1, 2, out);
  } else {
    /*int x, y, t;*/
    xs -= radius;
    fwrite(&xs, 1, 2, out);
    fwrite(&br, 1, 2, out);
    xs += radius;
    for (t = 0; t < 3; t++)
      for (y = 0; y < 16; y++)
        for (x = 0; x < br; x++) {
          writebrickpixel(x+xs, ys+ t*16+y, out);
        }
  }
}

int main() {
  
  FILE *outp = fopen("graphics.dat", "wb");

  int y, t, x;

  SDL_Init(SDL_INIT_VIDEO);

  int img_height = 8*16+8*3*16+73*16*3;

  display = SDL_CreateRGBSurface(SDL_SWSURFACE, 2* zinnenrad, img_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);

  brick = IMG_LoadPNG_RW(SDL_RWFromFile("graphics_brick.png", "rb"));
  zinne = IMG_LoadPNG_RW(SDL_RWFromFile("graphics_pinacle.png", "rb"));

  SDL_Rect r;
  r.w = 640;
  r.h = img_height;
  r.x = 0;
  r.y = 0;

  /* generate the slices */
  SDL_FillRect(display, &r, SDL_MapRGBA(display->format, 0,0,255,255));

  for (t = 0; t < 8; t++)
    createslice(wadd / 8 * t, t*16);

  for (t = 0; t < 8; t++)
    createzinne(t*16*3+ 8*16, wadd / 8 * t);

//  SavePNGImage("ttttest.png", display);
//  return;

  /* generate doors and save them */
  for (t = -36; t < 37; t++)
    createdoor(t * wadd / 8, 8*16 + 8*3*16 + (t+36)*16*3, radius, 30, radius, 3*hoehe, 0);


  SDL_Surface *disp2 = colorreduction(display, 256);
  SDL_FreeSurface(display);
  display = disp2;

  /* write out palette for bricks */
  for (t = 0; t < 256; t++) {
    Uint8 c;
    c = display->format->palette->colors[t].g;
    fwrite(&c, 1, 1, outp);
    c = display->format->palette->colors[t].r;
    fwrite(&c, 1, 1, outp);

    if (abs((int)display->format->palette->colors[t].g - (int)display->format->palette->colors[t].g) > 5)
      printf("oops color problems\n");
  }

  /* save the slices */
  for (t = 0; t < 8; t++)
    for (y = 0; y < 16; y++)
      for (x = 0; x < 2*radius; x++)
        writebrickpixel(x, t*16+y, outp);

  /* save the battlement */
  for (t = 0; t < 8; t++)
    for (y = 0; y < 3*16; y++)
      for (x = 0; x < 2*zinnenrad; x++)
        writebrickpixel(x, 8*16 + t*16*3+y, outp);

  /* save doors */
  for (t = -36; t < 37; t++) {
    getdoor( 8*16 + 8*3*16 + (t+36)*16*3, outp);
  }

  /* write out palette for environment */
  for (t = 0; t < 256; t++) {
    Uint8 c = t;
    fwrite(&c, 1, 1, outp);
    fwrite(&c, 1, 1, outp);
  }

  createcyl(steprad, outp);
  createcyl(elevatorrad, outp);
  createcyl(stickrad, outp);

  SDL_Quit();
}


