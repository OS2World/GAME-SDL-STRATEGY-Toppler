#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>

/*
 * this program loads a few picture files and creates a file with information
 * for the program for the scroller
 * the input pictures must be .PNG
 *
 * the data file should allow for any number of layers, each layer must be 240 pixel high
 * but can have a different width
 * you must specify the speed by which the image is moved with 2 numbers. the speed
 * will actually be the fraction of number one and two
 *
 * so you can create a standing background image by specifying a 0 for speed
 */

/*
 * ok this structure contains the data for one layer
 */
typedef struct layer {

  unsigned int width;
  unsigned int numerator, denominator;

  SDL_Surface * colors;
  SDL_Surface * mask;

  struct layer * next;
} layer;

/*
 * the layer list ancor
 */
layer * layer_ancor = NULL;

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
 * writes on layer to the file
 */
void write_layer(layer * l, FILE * out) {

  unsigned char c;
  int x, y;

  static int first = 0;

  Uint8 b;

  c = l->width >> 8;
  fwrite(&c, 1, 1, out);
  c = l->width & 0xff;
  fwrite(&c, 1, 1, out);

  c = l->numerator >> 8;
  fwrite(&c, 1, 1, out);
  c = l->numerator & 0xff;
  fwrite(&c, 1, 1, out);

  c = l->denominator >> 8;
  fwrite(&c, 1, 1, out);
  c = l->denominator & 0xff;
  fwrite(&c, 1, 1, out);

  write_palette(out, l->colors);

  for (y = 0; y < 480; y++)
    for (x = 0; x < l->width; x++) {
      b = get_color(l->colors, x, y);
      fwrite(&b, 1, 1, out);
      if (first) {
        b = get_alpha(l->mask, x, y);
        fwrite(&b, 1, 1, out);
      }
    }

  first = 1;
}

void save(layer * l, FILE * o) {

  if (l) {
    save(l->next, o);
    write_layer(l, o);
  }
}

int main(int argv, char* args[]) {

  int l;

  printf("start\n");
    
  if (argv < 3) return 1;

  int layers; // number of layers
  sscanf(args[1], "%i", &layers);
  printf("generating %i layers\n", layers);

  int towerpos;
  int towerspeed_num, towerspeed_den;
  sscanf(args[2], "%i", &towerpos);
  sscanf(args[3], "%i/%i", &towerspeed_num, &towerspeed_den);
  printf("putting tower as layer %i with speed %i/%i\n", towerpos, towerspeed_num, towerspeed_den);

  for (l = 0; l < layers; l++) {
    layer * la = (layer*)malloc(sizeof(layer));

    la->colors = IMG_LoadPNG_RW(SDL_RWFromFile(args[4+3*l], "rb"));
    la->mask = IMG_LoadPNG_RW(SDL_RWFromFile(args[4+3*l+1], "rb"));

    la->width = la->colors->w;

    sscanf(args[4+3*l+2], "%i/%i", &la->numerator, &la->denominator);

    printf("image %s with speed %i/%i\n", args[4+3*l], la->numerator, la->denominator);

    la->next = layer_ancor;
    layer_ancor = la;
  }

  printf("\n saving...\n");

  FILE * o = fopen("scroller.dat", "wb");
  char c;

  fwrite(&layers, 1, 1, o);
  fwrite(&towerpos, 1, 1, o);

  c = towerspeed_num >> 8;
  fwrite(&c, 1, 1, o);
  c = towerspeed_num & 0xff;
  fwrite(&c, 1, 1, o);

  c = towerspeed_den >> 8;
  fwrite(&c, 1, 1, o);
  c = towerspeed_den & 0xff;
  fwrite(&c, 1, 1, o);

  save(layer_ancor, o);

  return 0;
}



