#include "level.h"
#include <stdio.h>

/*
 * all right this program creates a mission from given towers
 *
 * first argument: mission name
 * rest: filenames of towers
 */

int main(int argn, char *args[]) {

  int i;

  printf("create %s\n", args[1]);
  lev_mission_new(args[1]);

  for (i = 2; i < argn; i++) {
    printf("add %s\n", args[i]);
    lev_mission_addtower(args[i]);
  }

  printf("finish\n");
  lev_mission_finish();
}
