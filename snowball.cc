#include "robots.h"
#include "level.h"
#include "points.h"
#include "sound.h"

static int an;
static long subKind;
static long ve;
static long time;

void snb_init(void) {
  time = -1;
}

/* move the snowball and check if it hits something */
void snb_movesnowball(void) {

  /* the snowball moves up this fiels specifies by how much */
  static long schusshoch[12] = {
    0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0
  };

  int nr;

  if (time < 0) return;

  if (time == 12) {
    time = -1;
    return;
  }

  time++;
  an = (an + subKind * 2) & 0x7f;
  ve += schusshoch[time - 1];

  if (!lev_testfigure(an, ve, -1, 0, 2, 2, 5)) {
    time = -1;
    return;
  }

  nr = rob_snowballkollision(an, ve);

  if (nr == -1)
    return;
  else {
    snd_hit();
    pts_add(rob_gothit(nr));
    time = -1;
  }
}

bool snb_exists(void) { return time != -1; }

void snb_start(int verticalpos, int anglepos, bool look_left) {

  ve = verticalpos;
  if (!look_left) {
    an = anglepos - 1;
    subKind = -1;
  } else {
    an = anglepos + 1;
    subKind = 1;
  }
  an &= 0x7f;
  time = 0;
}

int snb_verticalpos(void) { return ve; }
int snb_anglepos(void) { return an; }

