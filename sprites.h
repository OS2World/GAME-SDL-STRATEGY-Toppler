#ifndef SPRITES_H
#define SPRITES_H

#include <SDL.h>

/* coordinates a collection of sprites */

/* initializes with a maximum number of possible prites */
void spr_init(Uint16 maxsp);

/* frees all sprites */
void spr_done(void);

/* enters one sprite into the collection and returns a unique index */
long spr_savesprite(SDL_Surface *s);

/* returns the sprite data for the given index */
SDL_Surface *spr_spritedata(Uint16 nr);

#endif
