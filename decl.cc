#include "decl.h"

#include <SDL/SDL.h>

#include <stdlib.h>

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

FILE *open_data_file(char *name, char *par) {

  /* look into actual diractory */
  if (dcl_fileexists(name))
    return fopen(name, par);

  /* look into the data dir */
  char n[200];

  sprintf(n, DATADIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, par);

  return NULL;
}

FILE *open_global_config_file(char *name, char *par) {
  char n[200];

  sprintf(n, CONFIGDIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, par);

  if (dcl_fileexists(name))
    return fopen(name, par);

  return NULL;
}

FILE *open_local_config_file(char *name, char *par) {
  char n[200];

  sprintf(n, "%s/%s", getenv("HOME"), name);
  if (dcl_fileexists(n))
    return fopen(n, par);

  if (dcl_fileexists(name))
    return fopen(name, par);

  return NULL;
}

FILE *open_highscore_file(char *name, char *par) {
  char n[200];

  sprintf(n, HISCOREDIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, par);

  if (dcl_fileexists(name))
    return fopen(name, par);

  return NULL;
}

FILE *create_highscore_file(char *name, char *par) {
  char n[200];
  FILE *f;

  sprintf(n, HISCOREDIR"/%s", name);
  f = fopen(n, par);

  if (f) {
    /* set right owner and read write properties */
    return f;
  } else {
    return fopen(name, par);
  }

  return NULL;
}

