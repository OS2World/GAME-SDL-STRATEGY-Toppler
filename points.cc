#include "points.h"

static unsigned int points;
static unsigned long nextlife;
static int lifes;

#define LIFE_INCREMENT 5000

void pts_reset(void) {
  points = 0;
  lifes = 3;
  nextlife = LIFE_INCREMENT;
}

void pts_add(int add) {
  points += add;

  while (points >= nextlife) {
    lifes++;
    nextlife += LIFE_INCREMENT;
    if (lifes > 8)
      lifes = 8;
  }
}

unsigned int pts_points(void) {
  return points;
}

unsigned char pts_lifes(void) {
  return lifes;
}

void pts_died(void) {
  lifes--;
}

bool pts_lifesleft(void) {
  return lifes != 0;
}

