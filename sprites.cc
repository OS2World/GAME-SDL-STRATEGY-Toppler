#include "sprites.h"

#include "decl.h"

#include <stdlib.h>

static int maxsprite = 0;
static SDL_Surface **sarray = NULL;

void spr_init(int maxsp) {
  assert(maxsprite == 0, "sprite init called more than once\n");

  maxsprite = maxsp;

  sarray = new (SDL_Surface *)[maxsp];

  for (int i = 0; i < maxsprite; i++)
    sarray[i] = NULL;
}

void spr_done() {

  delete [] sarray;

  sarray = NULL;
  maxsprite = 0;
}

long spr_savesprite(SDL_Surface *s) {

  long t;

  t = 0;
  while ((sarray[t] != NULL) && (t < maxsprite))
    t++;

  if (t < maxsprite) {
    sarray[t] = s;
    return t;
  } else
    return -1;
}

SDL_Surface *spr_spritedata(long nr) {

  if (nr < maxsprite)
    return sarray[nr];
  else
    return NULL;
}

