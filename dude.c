#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>

Uint8 get_color(SDL_Surface *s, int x, int y) {
  return ((Uint8*)s->pixels)[y*s->pitch+x];
}

Uint8 get_alpha(SDL_Surface *s, int x, int y) {
  return s->format->palette->colors[((Uint8*)s->pixels)[y*s->pitch+x]].r;
}

void getpixel(Uint16 s, Uint16 z, Uint8 fx, Uint8 fy, Uint16 x, Uint16 y,
              FILE *out, SDL_Surface *col, SDL_Surface *msk)
{
  if (fx)
    x = s + 36 - x;
  else
    x = s + x;

  if (fy)
    y = z + 39 - y;
  else
    y = z + y;

  Uint8 c;

  c = get_color(col, x, y);
  fwrite(&c, 1, 1, out);

  c = get_alpha(msk, x, y);
  fwrite(&c, 1, 1, out);

}

void get(Uint16 s, Uint16 z, Uint8 fx, Uint8 fy, FILE *out, SDL_Surface *col, SDL_Surface *msk)
{
  unsigned short x, y;

  for (y = 0; y < 40; y++)
    for (x = 0; x < 40; x++) {
      getpixel(s, z, fx, fy, x, y, out, col, msk);
    }
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

main(int argc, char *argv[])
{
  unsigned char p[400];

  SDL_Surface * colors = IMG_LoadPNG_RW(SDL_RWFromFile("dude_colors.png", "rb"));
  SDL_Surface * mask = IMG_LoadPNG_RW(SDL_RWFromFile("dude_mask.png", "rb"));

  FILE *outp = fopen("dude.dat", "wb");

  write_palette(outp, colors);

  get(2+38*0, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*1, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*2, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*3, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*4, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*5, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*6, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*7, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*8, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*9, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*10, 2+42*0, 0, 0, outp, colors, mask);
  get(2+38*11, 2+42*0, 0, 0, outp, colors, mask);

  get(2+38*0, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*1, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*2, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*3, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*4, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*5, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*6, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*7, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*8, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*9, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*10, 2+42*1, 0, 0, outp, colors, mask);
  get(2+38*11, 2+42*1, 0, 0, outp, colors, mask);

  get(2+38*0, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*1, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*2, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*3, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*4, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*5, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*6, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*7, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*8, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*9, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*10, 2+42*2, 0, 0, outp, colors, mask);
  get(2+38*11, 2+42*2, 0, 0, outp, colors, mask);

  get(2+38*0, 2+42*3, 0, 0, outp, colors, mask);

  get(2+38*0, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*1, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*2, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*3, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*4, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*5, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*6, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*7, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*8, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*9, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*10, 2+42*0, 1, 0, outp, colors, mask);
  get(2+38*11, 2+42*0, 1, 0, outp, colors, mask);

  get(2+38*0, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*1, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*2, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*3, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*4, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*5, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*6, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*7, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*8, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*9, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*10, 2+42*1, 1, 0, outp, colors, mask);
  get(2+38*11, 2+42*1, 1, 0, outp, colors, mask);

  get(2+38*0, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*1, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*2, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*3, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*4, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*5, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*6, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*7, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*8, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*9, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*10, 2+42*2, 1, 0, outp, colors, mask);
  get(2+38*11, 2+42*2, 1, 0, outp, colors, mask);

  get(2+38*0, 2+42*3, 1, 0, outp, colors, mask);

  fclose(outp);
}

