#include "points.h"

static unsigned short points;
static int lifes;


void pts_reset() {
  points = 0;
  lifes = 3;
}

void pts_add(int add) {
  unsigned short t;

  t = points / 1000 % 10;
  points += add;

  if ((t != 4) && (t != 9) || (t == points / 1000 % 10))
    return;

  lifes++;
  if (lifes == 9)
    lifes = 8;
}

unsigned int pts_points() {
  return points;
}

unsigned char pts_lifes() {
  return lifes;
}

void pts_died() {
  lifes--;
}

bool pts_lifesleft() {
  return lifes != 0;
}

