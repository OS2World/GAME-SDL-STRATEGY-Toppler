#include "elevators.h"

#include "level.h"
#include "decl.h"
#include "toppler.h"

#include <stdlib.h>

#define MAX_ELE 4

/* elevators are handled in a quite complicated manner: while
 the toppler is moving on the elevator the platform is invisible
 and frawn together with the toppler. The stick below the elevator
 is always drawn in such a way that the last one will appear or disappear
 below the platform under the toppler */

static struct {
  /* the current position of the platform */
  int angle;
  int vertical;

  /* time until the elevator falls down */
  int time;

  /* backgound value necesary because in between the stations it
   is impossible to show a platform so we save the actual value
   there and force a station at the position, when the elevator moves
   further down we restore the value there */
  unsigned char bg;
} elevators[MAX_ELE];

static int active_ele;
static int ele_dir;

void ele_init(void) {

  for (int t = 0; t < MAX_ELE; t++)
    elevators[t].time = -1;

  active_ele = -1;

}

void ele_select(int row, int col) {

  assert(active_ele == -1, "select more than one elevator\n");

  int what = 0;

  col /= 8;
  row /= 4;
  row--;

  for (int t = 0; t < MAX_ELE; t++) {
    if ((elevators[t].time == -1) && (what == 0)) {
      what = 1;
      active_ele = t;
    }
    if ((elevators[t].angle == col) && (elevators[t].vertical == row)) {
      what = 2;
      active_ele = t;
    }
  }

  elevators[active_ele].angle = col;
  elevators[active_ele].vertical = row;
  elevators[active_ele].time = -1;

}

void ele_activate(int dir) {

  assert(active_ele != -1, "work with unselected elevator, activate\n");

  lev_platform2stick(elevators[active_ele].vertical, elevators[active_ele].angle);
  ele_dir = dir;
  if (dir == -1) ele_move();
}

void ele_move(void) {

  assert(active_ele != -1, "work with unselected elevator, move\n");

  if (ele_dir == 1) {
    elevators[active_ele].vertical++;
  
    lev_empty2stick(elevators[active_ele].vertical, elevators[active_ele].angle);
  } else {
    lev_stick2empty(elevators[active_ele].vertical, elevators[active_ele].angle);
  
    elevators[active_ele].vertical--;
  }
}

bool ele_is_atstop(void) {

  assert(active_ele != -1, "work with unselected elevator, is_at_stop\n");

  if (ele_dir == 1)
    return lev_is_station(elevators[active_ele].vertical + 1, elevators[active_ele].angle);
  else
    return lev_is_station(elevators[active_ele].vertical, elevators[active_ele].angle);
}

void ele_deactivate(void) {

  assert(active_ele != -1, "deselected an inactive elevator\n");

  if (ele_dir == 1) ele_move();

  lev_stick2platform(elevators[active_ele].vertical, elevators[active_ele].angle);

  int ae = active_ele;
  active_ele = -1;

  if (!lev_is_station(elevators[ae].vertical, elevators[ae].angle))
    elevators[ae].bg = lev_putplatform(elevators[ae].vertical, elevators[ae].angle);
  else
    if (lev_is_bottom_station(elevators[ae].vertical, elevators[ae].angle))
      return;
    else
      elevators[ae].bg = lev_tower(elevators[ae].vertical, elevators[ae].angle);

  elevators[ae].time = 0x7d;
}

void ele_update(void) {

  for (int t = 0; t < MAX_ELE; t++) {
    if (elevators[t].time == 0) {
      lev_restore(elevators[t].vertical, elevators[t].angle, elevators[t].bg);
      lev_platform2empty(elevators[t].vertical, elevators[t].angle);
      elevators[t].vertical--;
      lev_stick2platform(elevators[t].vertical, elevators[t].angle);
      if (lev_is_bottom_station(elevators[t].vertical, elevators[t].angle)) {
        elevators[t].time = -1;
        top_sidemove();
      } else
        elevators[t].bg = lev_putplatform(elevators[t].vertical, elevators[t].angle);
    }

    if (elevators[t].time > 0)
      elevators[t].time--;
  }
}

