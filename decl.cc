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
  return true;
}

