#include "bonus.h"
#include "keyb.h"
#include "screen.h"
#include "decl.h"
#include "points.h"
#include "sprites.h"
#include "palette.h"
#include "level.h"

#include <stdlib.h>

#define fishcnt         15
#define gametime        500
#define scrollerspeed   2

static long fish[fishcnt + 1][4];
static long torpedox, torpedoy, subposx, subposy;
static int substat;

static bool escape(long time, long x) {
  long towerpos;
  int b;

  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_bonusgame);

  scr_putbar(0, 0, 320, 240);

  if (time < 300)
    towerpos = -(2*time);
  else
    towerpos = gametime * scrollerspeed - 2*time;

  scr_draw_bonus1(x, towerpos);

  if (torpedox != -1)
    scr_draw_torpedo(torpedoy, torpedox);
  scr_draw_submarine(subposy, subposx, (time & 1) * 3 + substat);

  for (b = 0; b <= fishcnt; b++) {
    if (fish[b][0] >= 0)
      scr_draw_fish(fish[b][1], fish[b][0], fish[b][2]);

  }
  scr_draw_bonus2(x, towerpos);

  scr_writetext_center(61, "REALLY QUIT?");
  scr_writetext_center(95,  "  ESC: YES, QUIT");
  scr_writetext_center(112, "OTHER: NO PLAY");

  scr_swap();

  Uint8 inp = key_chartyped();

  do {
    inp = key_chartyped();
  } while (!inp);

  if (inp == 27)
    return true;
  else
    pal_colors(pal_bonusgame);

  return false;
}

static void pause(long time, long x) {
  long towerpos;
  int b;

  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_bonusgame);

  scr_putbar(0, 0, 320, 240);

  if (time < 300)
    towerpos = -(2*time);
  else
    towerpos = gametime * scrollerspeed - 2*time;

  scr_draw_bonus1(x, towerpos);

  if (torpedox != -1)
    scr_draw_torpedo(torpedoy, torpedox);
  scr_draw_submarine(subposy, subposx, (time & 1) * 3 + substat);

  for (b = 0; b <= fishcnt; b++) {
    if (fish[b][0] >= 0)
      scr_draw_fish(fish[b][1], fish[b][0], fish[b][2]);

  }
  scr_draw_bonus2(x, towerpos);

  scr_writetext_center(61, "PAUSE");
  scr_writetext_center(95, "PRESS SPACE");

  scr_swap();
  do {
    dcl_wait();
  } while (!key_keypressed(fire_key));
  pal_colors(pal_bonusgame);
}


bool bns_game(void) {

  static unsigned short x;
  

  unsigned short time, nextfish;
  bool automatic = false;
  long towerpos;

  int b;

  subposx = 160-30;
  subposy = 60;

  for (b = 0; b <= fishcnt; b++)
    fish[b][0] = -1;

  torpedox = -1;

  time = 0;
  nextfish = 30;

  x = 0;

  pal_colors(pal_bonusgame);

  key_readkey();

  do {
    
    if (torpedox >= 0) {
      torpedox += 4;
      if (torpedox > 330)
        torpedox = -1;
      for (b = 0; b <= fishcnt; b++) {
        if (fish[b][0] > 0 && fish[b][2] >= 8) {
          if ((torpedox > fish[b][0] - 12) && (torpedox < fish[b][0] + 16) &&
              (torpedoy > fish[b][1] - 4) && (torpedoy < fish[b][1] + 16)) {
            torpedox = -1;
            fish[b][2] -= 8;
          }
        }
      }
    }

    if (!automatic) {
      if (key_keypressed(fire_key)) {
        if (torpedox == -1) {
          torpedox = subposx + 50;
          torpedoy = subposy + 5;
        }
      }
  
      if ((bool)(key_keystat() & down_key)) {
        substat = 2;
        if (subposy < 130)
          subposy += 2;
      } else {
        if ((bool)(key_keystat() & up_key)) {
          substat = 0;
          if (subposy > 60)
            subposy -= 2;
        } else
          substat = 1;
      }
  
      if ((bool)(key_keystat() & left_key)) {
        if (subposx > 0)
          subposx -= 4;
      } else {
        if ((bool)(key_keystat() & right_key)) {
          if (subposx < 100)
            subposx += 2;
        } else {
          if ((subposx & 1) == 1)
            subposx++;
        }
      }
    } else {
      if (subposx > 160 - 30)
        subposx -= 4;
      else if (subposx < 160 -30)
        subposx += 2;

      if (subposy > 60)
        subposy -= 2;
    }

    if (key_keypressed(break_key))
      if (escape(time, x)) {
        return false;
      }

    if (key_keypressed(pause_key))
      pause(time, x);

    key_readkey();

    for (b = 0; b <= fishcnt; b++) {
      if (fish[b][0] >= 0) {
        fish[b][0] -= 2;
        fish[b][1] += fish[b][3];
        if (fish[b][1] > 150 || fish[b][1] < 40)
          fish[b][3] = -fish[b][3];
    
        if (fish[b][2] >= 8)
          fish[b][2] = ((fish[b][2] + 1) & 7) + 8;
        else
          fish[b][2] = (fish[b][2] + 1) & 7;

        if ((fish[b][2] < 8) &&
            (fish[b][0] > subposx - 20) &&
            (fish[b][0] < subposx + 60) &&
            (fish[b][1] > subposy - 20) &&
            (fish[b][1] < subposy + 20)) {
          pts_add(50);
          fish[b][0] = -1;
        }
      }
    }

    if (nextfish > 0)
      nextfish--;
    else {
      for (b = 0; b <= fishcnt; b++) {
        if (fish[b][0] < 0) {
          fish[b][0] = 320;
          fish[b][1] = rand() / (RAND_MAX / 70) + 60;
          fish[b][2] = 8;
          do {
            fish[b][3] = rand() / (RAND_MAX / 5) - 2;
          } while (fish[b][3] == 0);
          nextfish = rand() / (RAND_MAX / 20) + 5;
          break;
        }
      }
    }

    scr_putbar(0, 0, 320, 240);

    if (time < 300)
      towerpos = -(2*time);
    else
      towerpos = gametime * scrollerspeed - 2*time;

    scr_draw_bonus1(x, towerpos);

    if (torpedox != -1)
      scr_draw_torpedo(torpedoy, torpedox);
    scr_draw_submarine(subposy, subposx, (time & 1) * 3 + substat);

    for (b = 0; b <= fishcnt; b++) {
      if (fish[b][0] >= 0)
        scr_draw_fish(fish[b][1], fish[b][0], fish[b][2]);

    }
    scr_draw_bonus2(x, towerpos);

    scr_swap();

    if (x == 300) {
      pal_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());
      pal_calcdark(pal_towergame);
      pal_colors(pal_bonusgame);
    }

    if (time == gametime) {
      automatic = true;
      if ((subposx == 160-30) && (subposy == 60)) break;
    } else {
      x +=2;
      time++;
    }
    dcl_wait();

  } while (true);

  return true;
}
