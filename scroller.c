#include <SDL.h>
#include <SDL_image.h>

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

  SDL_Surface * image;

  struct layer * next;
} layer;

/*
 * the layer list ancor
 */
layer * layer_ancor = NULL;

/*
 * writes on layer to the file
 */
void write_layer(layer * l, FILE * out) {

  unsigned char c;
  int x, y;

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

  for (y = 0; y < 240; y++)
    for (x = 0; x < l->width; x++)
      fwrite(&(((char*)l->image->pixels)[y*l->image->pitch+x]), 1, 1, out);
}

void save(layer * l, FILE * o) {
  if (l) {
    save(l->next, o);
    write_layer(l, o);
  }
}

int main(int argv, char* args[]) {

  int i, l;

  printf("start\n");

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

    la->image = IMG_LoadPNG_RW(SDL_RWFromFile(args[4+2*l], "rb"));

    la->width = la->image->w;

    sscanf(args[4+2*l+1], "%i/%i", &la->numerator, &la->denominator);

    printf("image %s with speed %i/%i\n", args[4+2*l], la->numerator, la->denominator);

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

  for (i = 0; i < 152; i++) {
    c = layer_ancor->image->format->palette->colors[i].r;
    fwrite(&c, 1, 1, o);
    c = layer_ancor->image->format->palette->colors[i].g;
    fwrite(&c, 1, 1, o);
    c = layer_ancor->image->format->palette->colors[i].b;
    fwrite(&c, 1, 1, o);
  }

  save(layer_ancor, o);

  return 0;
}



