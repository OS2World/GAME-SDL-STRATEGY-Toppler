#include "points.h"

static unsigned int points;
static unsigned long nextlife;
static int lifes;

#define LIFE_INCREMENT 5000

void pts_reset() {
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

