#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

/* creates a font.dat file out of specially formatted image files
 * there must be 2 files: one containing the color for each pixel
 * belonging to a letter, one containing a mast having the alpha value
 * for each pixel
 *
 * the colors files must contain additional information about the size
 * of each letter in form of specially places pixels. the color of the
 * marker pixels is defines by the pixel in the top left corner of the image
 * the best is, if you have a look at the example image
 */

int ypos, xpos;
Uint8 fontheight = 1;
Uint8 markercolor;
FILE *outp;

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

void get(SDL_Surface *colors, SDL_Surface * mask, char c)
{
  // xpos und ypos zeigen auf oberes linkes umrandungs pixel */
  // hoehe ist bekannt : 19
  // breite ist zu suchen

  int height = fontheight;
  unsigned char width = 0;
  int i, x, y;
  Uint8 b;

  while (get_color(colors, xpos + width + 1, ypos + 1) != markercolor)
    width++;

  i = 0;

  fwrite(&c, 1, 1, outp);
  fwrite(&width, 1, 1, outp);

  for (y = 0; y < height; y++)
    for (x = 0; x < width; x++) {
      b = get_color(colors, xpos + 1 + x, ypos + 1 + y);
      fwrite(&b, 1, 1, outp);
      b = get_alpha(mask, xpos + 1 + x, ypos + 1 + y);
      fwrite(&b, 1, 1, outp);
    }

  xpos += (1 + width);

  while (get_color(colors, xpos + 1, ypos + 1) == markercolor) xpos++;

  if (get_color(colors, xpos + 1, ypos) != markercolor) {
    ypos += 1 + height;
    xpos = 0;
  }
}

int main() {

  SDL_Surface *colors = IMG_LoadPNG_RW(SDL_RWFromFile("font_colors.png", "rb"));
  SDL_Surface *mask = IMG_LoadPNG_RW(SDL_RWFromFile("font_mask.png", "rb"));

  markercolor = get_color(colors, 0, 0);

  outp = fopen("font.dat", "wb");

  write_palette(outp, colors);

  /* ok first lets fins out the pixel height of the fonts, I
   assume that all the rows have the same hight, so the first is ok */

  while (get_color(colors, 1, 1+fontheight) != markercolor)
    fontheight++;

  fwrite(&fontheight, 1, 1, outp);

  /* and not lets get the pixels */
  xpos = ypos = 0;

  get(colors, mask, '0');
  get(colors, mask, '1');
  get(colors, mask, '2');
  get(colors, mask, '3');
  get(colors, mask, '4');
  get(colors, mask, '5');
  get(colors, mask, '6');
  get(colors, mask, '7');
  get(colors, mask, '8');
  get(colors, mask, '9');
  get(colors, mask, 'A');
  get(colors, mask, 'B');
  get(colors, mask, 'C');
  get(colors, mask, 'D');
  get(colors, mask, 'E');
  get(colors, mask, 'F');
  get(colors, mask, 'G');
  get(colors, mask, 'H');
  get(colors, mask, 'I');
  get(colors, mask, 'J');
  get(colors, mask, 'K');
  get(colors, mask, '.');
  get(colors, mask, 'M');
  get(colors, mask, 'N');
  get(colors, mask, 'O');
  get(colors, mask, 'P');
  get(colors, mask, 'Q');
  get(colors, mask, 'R');
  get(colors, mask, 'S');
  get(colors, mask, 'T');
  get(colors, mask, 'U');
  get(colors, mask, 'V');
  get(colors, mask, 'W');
  get(colors, mask, 'X');
  get(colors, mask, 'Y');
  get(colors, mask, 'Z');
  get(colors, mask, 'a');
  get(colors, mask, 'b');
  get(colors, mask, 'c');
  get(colors, mask, 'd');
  get(colors, mask, 'e');
  get(colors, mask, 'f');
  get(colors, mask, 'h');
  get(colors, mask, 'i');
  get(colors, mask, 'j');
  get(colors, mask, 'k');
  get(colors, mask, 'l');
  get(colors, mask, 'm');
  get(colors, mask, 'n');
  get(colors, mask, 'o');
  get(colors, mask, 'p');
  get(colors, mask, 'q');
  get(colors, mask, 'r');
  get(colors, mask, 's');
  get(colors, mask, 't');
  get(colors, mask, 'u');
  get(colors, mask, 'v');
  get(colors, mask, 'w');
  get(colors, mask, 'x');
  get(colors, mask, 'y');
  get(colors, mask, 'z');
  get(colors, mask, 'L');
  get(colors, mask, ',');
  get(colors, mask, '?');
  get(colors, mask, '+');
  get(colors, mask, '*');
  get(colors, mask, 'g');
  get(colors, mask, ':');
  get(colors, mask, ';');
  get(colors, mask, '(');
  get(colors, mask, ')');
  get(colors, mask, '!');
  get(colors, mask, '/');
  get(colors, mask, '%');
  get(colors, mask, '&');
  get(colors, mask, '\'');
  get(colors, mask, '"');
  get(colors, mask, '\x01'); /* fonttoppler */
  get(colors, mask, '\x02'); /* fontpoint */

  fclose(outp);
}

