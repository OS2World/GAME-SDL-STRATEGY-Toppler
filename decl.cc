#include "decl.h"

#include <SDL/SDL.h>

static long last;

bool fullscreen;
bool nosound;

void dcl_wait() {
  while ((SDL_GetTicks() - last) < 55*1 ) SDL_Delay(2);
  last = SDL_GetTicks();
}

bool dcl_fileexists(char *n) {
  FILE *f = fopen(n, "r");

  if (f) {
    fclose(f);
    return true;
  } else
    return false;
}

FILE *file_open(char *name, char *par) {

  /* look into actual diractory */
  if (dcl_fileexists(name))
    return fopen(name, par);

  /* look into the data dir */
  char n[200];

  sprintf(n, DATADIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, par);

  assert(false, "could not open file");
}
