#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>

#include <math.h>

#include "pngsaver.h"

Uint8 get_color(SDL_Surface *s, int x, int y) {
  return ((Uint8*)s->pixels)[y*s->pitch+x];
}

Uint8 get_alpha(SDL_Surface *s, int x, int y) {
  return s->format->palette->colors[((Uint8*)s->pixels)[y*s->pitch+x]].r;
}

void write_palette(FILE *out, SDL_Surface *s) {
  int i;
  Uint8 c = s->format->palette->ncolors - 1;
  
  fwrite(&c, 1, 1, out);

  for (i = 0; i < s->format->palette->ncolors; i++) {
    fwrite(&s->format->palette->colors[i].r, 1, 1, out);
    fwrite(&s->format->palette->colors[i].g, 1, 1, out);
    fwrite(&s->format->palette->colors[i].b, 1, 1, out);
  }
}


/*
 * rensers a curve with the following property:
 *
 * x(t) = (cos(t))^n
 * y(t) = (sin(t))^n
 *
 * t(x) = acos(x ^ (1/n))
 * y(x) = (sin(acos(x^(1/n))))^n  (-1 <= x <= 1)
 *
 */


void render_star(SDL_Surface *s, Uint16 size, double power) {
  Uint16 x, y, t, z;
  double alias;

  Uint16 rad = size / 2;

  for (x = 0; x < rad; x++)
    for (y = 0; y < rad; y++) {
      alias = 0;
      for (t = 0; t < 8; t++)
        for (z = 0; z < 8; z++) {
          double dx = x * 8 + t;
          double dy = y * 8 + z;

          dx /= ((rad+1) * 8);
          dy /= ((rad+1) * 8);

          double o = pow(sin(acos(pow(dx, 1/power))), power);

          if (dy < o)
            alias++;
        }

      alias = alias * 255 / 64 + 0.5;


      ((Uint8*)s->pixels)[y*s->pitch+x] = 255-(Uint8)alias;
      ((Uint8*)s->pixels)[(size-2-y)*s->pitch+x] = 255-(Uint8)alias;
      ((Uint8*)s->pixels)[y*s->pitch+size-2-x] = 255-(Uint8)alias;
      ((Uint8*)s->pixels)[(size-2-y)*s->pitch+size-2-x] = 255-(Uint8)alias;
    }
}

void xchg(SDL_Surface *s)
{
  int i;

  s->format->palette->colors[0x11] = s->format->palette->colors[0];

  for (i = 0; i < 320*200; i++) {
    if (((Uint8*)s->pixels)[i] == 0)
      ((Uint8*)s->pixels)[i] = 0x11;
    else
      if (((Uint8*)s->pixels)[i] == 0x11)
        ((Uint8*)s->pixels)[i] = 0;
  }
}


unsigned char getpixel(SDL_Surface *s, Uint16 x, Uint16 y)
{
  return ((Uint8*)(s->pixels))[y*s->pitch+x*s->format->BytesPerPixel];
}

long bitbuf;
long bitpos;
FILE *outf;

struct LOC_putbits {
  unsigned char b;
} ;


void putbits(unsigned short w, unsigned char b_)
{
  struct LOC_putbits V;

  V.b = b_;
  bitbuf = (bitbuf << V.b) | w;
  bitpos += V.b;
  while (bitpos >= 8) {
    V.b = (((unsigned long)bitbuf) >> (bitpos - 8)) & 0xff;
    fwrite(&V.b, 1, 1, outf);
    bitpos -= 8;
  }
}


int main() {

  Uint16 s, n, p, x, y;

  SDL_Surface * screen;

  outf = fopen("sprites.dat", "wb");

  bitpos = 0;

  SDL_Surface *colors = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_robots_colors.png", "rb"));
  SDL_Surface *mask = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_robots_mask.png", "rb"));

  write_palette(outf, colors);

  printf("robots\n");

  for (s = 0; s < 8; s++) {
    for (n = 0; n < 16; n++) {
      for (y = 0; y < 32; y++) {
        for (x = 0; x < 32; x++) {
          Uint8 b;

          b = get_color(colors, n*32+x, s*32+y);
          fwrite(&b, 1, 1, outf);
          b = get_alpha(mask, n*32+x, s*32+y);
          fwrite(&b, 1, 1, outf);
        }
      }
    }
  }

  SDL_FreeSurface(colors);
  SDL_FreeSurface(mask);

  colors = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_balls_colors.png", "rb"));
  mask = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_balls_mask.png", "rb"));

  printf("balls\n");

  write_palette(outf, colors);

  for (n = 0; n < 2; n++) {
    for (y = 0; y < 32; y++) {
      for (x = 0; x < 32; x++) {
        Uint8 b;

        b = get_color(colors, n*32+x, y);
        fwrite(&b, 1, 1, outf);
        b = get_alpha(mask, n*32+x, y);
        fwrite(&b, 1, 1, outf);
      }
    }
  }

  SDL_FreeSurface(colors);
  SDL_FreeSurface(mask);

  colors = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_box_colors.png", "rb"));
  mask = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_box_mask.png", "rb"));

  printf("box\n");

  write_palette(outf, colors);

  for (n = 0; n < 16; n++) {
    for (y = 0; y < 16; y++) {
      for (x = 0; x < 16; x++) {
        Uint8 b;

        b = get_color(colors, n*16+x, y);
        fwrite(&b, 1, 1, outf);
        b = get_alpha(mask, n*16+x, y);
        fwrite(&b, 1, 1, outf);
      }
    }
  }

  SDL_FreeSurface(colors);
  SDL_FreeSurface(mask);

  colors = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_snowball_colors.png", "rb"));
  mask = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_snowball_mask.png", "rb"));

  printf("snowball\n");

  write_palette(outf, colors);

  for (y = 0; y < 16; y++) {
    for (x = 0; x < 16; x++) {
        Uint8 b;

        b = get_color(colors, x, y);
        fwrite(&b, 1, 1, outf);
        b = get_alpha(mask, x, y);
        fwrite(&b, 1, 1, outf);
    }
  }

  SDL_FreeSurface(colors);
  SDL_FreeSurface(mask);

  printf("star\n");

  SDL_Surface *src = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 8, 0, 0, 0, 0);

  for (n = 0; n < 256; n++) {
    src->format->palette->colors[n].r = n;
    src->format->palette->colors[n].g = n;
    src->format->palette->colors[n].b = n;
  }

  write_palette(outf, src);


  for (n = 0; n < 16; n++) {
    render_star(src, 32, 3 * exp((-1.0*n)/5));
    for (y = 0; y < 32; y++) {
      for (x = 0; x < 32; x++) {
          Uint8 b;
  
          b = 255;
          fwrite(&b, 1, 1, outf);
          b = get_alpha(src, x, y);
          fwrite(&b, 1, 1, outf);
      }
    }
  }

  SDL_FreeSurface(src);


  screen = IMG_LoadPNG_RW(SDL_RWFromFile("sprites_bonus.png", "rb"));
  xchg(screen);
  printf("bonus\n");

  for (p = 1; p <= 23; p++) {
    putbits(screen->format->palette->colors[p].r, 8);
    putbits(screen->format->palette->colors[p].g, 8);
    putbits(screen->format->palette->colors[p].b, 8);
  }

  /* fisch */
  printf("fisch\n");
  for (n = 0; n <= 1; n++) {
    for (s = 0; s <= 7; s++) {
      for (y = 0; y <= 19; y++) {
        for (x = 0; x <= 19; x++) {
          putbits(getpixel(screen, s * 20 + x, y + n * 20), 6);
        }
      }
    }
  }

  /* submarine */
  printf("submarine\n");
  for (n = 0; n < 2; n++) {
    for (s = 0; s < 4; s++) {
      for (y = 0; y < 39; y++) {
        for (x = 0; x < 50; x++) {
          putbits(getpixel(screen, s*51 + x, n* 40 + y + 121), 6);
        }
      }
    }
  }

  /* torpedo */
  printf("torpedo\n");
  for (y = 0; y <= 2; y++) {
    for (x = 0; x <= 15; x++) {
      putbits(getpixel(screen, x, y + 107), 6);
    }
  }

  while (bitpos >= 8) {
    x = (((unsigned long)bitbuf) >> (bitpos - 8)) & 0xff;
    fwrite(&x, 1, 1, outf);
    bitpos -= 8;
  }
  if (bitpos > 0) {
    x = (bitbuf << (8 - bitpos)) & 0xff;
    fwrite(&x, 1, 1, outf);
  }

  fclose(outf);
}



