#include "stars.h"

#include "sprites.h"
#include "screen.h"

#include "SDL.h"
#include "stdlib.h"

#define starstep 5

static unsigned short starnr;

static struct {
  long x, y;
} stars[21];

void sts_draw(void)
{
  SDL_Surface *p = spr_spritedata((long)starnr);

  for (int t = 0; t <= 20; t++)
    scr_blit(p, stars[t].x, stars[t].y);
}

void sts_init(int sn) {
  for (int t = 0; t <= 20; t++) {
    stars[t].x = rand() / (RAND_MAX / 320) - 16;
    stars[t].y = rand() / (RAND_MAX / 240) - 16;
  }

  starnr = sn;
}

void sts_move(long x, long y)
{
  int t;

  for (t = 0; t <= 20; t++) {
    stars[t].x += starstep * x;
    stars[t].y += y;
  }
  for (t = 0; t <= 20; t++) {
    if (stars[t].x > 320) {
      stars[t].x = rand() / (RAND_MAX / starstep) + 16;
      stars[t].y = rand() / (RAND_MAX / 240);
    } else {
      if (stars[t].x < -16) {
        stars[t].x = 320 - rand() / (RAND_MAX / starstep);
        stars[t].y = rand() / (RAND_MAX / 240);
      }
    }

    if (stars[t].y > 240) {
      stars[t].y = -16;
      stars[t].x = rand() / (RAND_MAX / 336) - 16;
    } else {
      if (stars[t].y < -16) {
        stars[t].y = 240;
        stars[t].x = rand() / (RAND_MAX / 336) - 16;
      }
    }
  }
}

