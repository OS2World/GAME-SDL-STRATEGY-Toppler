/* Tower Toppler - Nebulus
 * Copyright (C) 2000-2002  Andreas Röver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "bonus.h"
#include "keyb.h"
#include "screen.h"
#include "menu.h"
#include "decl.h"
#include "points.h"
#include "sprites.h"
#include "level.h"

#include <stdlib.h>

#define fishcnt         15
#define gametime        500
#define scrollerspeed   2

static struct {
   Sint32 x;
   Sint32 y;
   Sint32 state;
   Sint32 ydir;
} fish[fishcnt + 1];

static Sint32 torpedox, torpedoy, subposx, subposy;

static Sint32 callback_time;
static Sint32 callback_x;

/* callback proc for men_yn() */
static void
bonus_background_proc(void)
{
  Sint32 towerpos;

  scr_putbar(0, 0, SCREENWID, SCREENHEI, 0, 0, 0, 255);

  if (callback_time < 300)
    towerpos = -(4*callback_time);
  else
    towerpos = gametime * scrollerspeed - 4*callback_time + SCREENWID + (SPR_SLICEWID*2);

  scr_draw_bonus1(callback_x, towerpos);

  if (torpedox != -1)
    scr_draw_torpedo(torpedoy, torpedox);
  scr_draw_submarine(subposy - 20, subposx, callback_time % 9);

  for (Uint8 b = 0; b <= fishcnt; b++) {
    if (fish[b].x >= -SPR_FISHWID)
      scr_draw_fish(fish[b].y, fish[b].x, fish[b].state);

  }
  scr_draw_bonus2(callback_x, towerpos);
}

static bool 
escape(Sint32 time, Uint32 x)
{
  key_wait_for_any();

  callback_time = time;
  callback_x = x;

  set_men_bgproc(bonus_background_proc);

  if (men_yn("Really quit", false))
    return true;

  return false;
}

static void pause(Sint32 time, Uint32 x) {
   
  callback_time = time;
  callback_x = x;
  set_men_bgproc(bonus_background_proc);

  men_info("Pause", -1, 1);
}


bool bns_game(void) {

  static Uint32 xpos = 0;
  
  Uint32 time, nextfish;
  bool automatic = false;
  Sint32 towerpos;

  Uint8 b;

  subposx = SUBM_TARGET_X;
  subposy = SUBM_TARGET_Y;

  for (b = 0; b <= fishcnt; b++)
    fish[b].x = -(SPR_FISHWID+1);

  torpedox = -1;

  time = 0;
  nextfish = 30;

  set_men_bgproc(NULL);

  key_readkey();

  do {
    
    if (torpedox >= 0) {
      torpedox += 8;
      if (torpedox > (SCREENWID+SPR_TORPWID))
        torpedox = -1;
      for (b = 0; b <= fishcnt; b++) {
        if (fish[b].x > 0 && fish[b].state >= 32) {
          if ((torpedox + SPR_TORPWID > fish[b].x) && (torpedox < fish[b].x + SPR_FISHWID) &&
              (torpedoy + SPR_TORPHEI > fish[b].y) && (torpedoy < fish[b].y + SPR_FISHHEI)) {
            torpedox = -1;
            fish[b].state -= 32;
          }
        }
      }
    }

    if (!automatic) {
      if (key_keypressed(fire_key)) {
        if (torpedox == -1) {
          torpedox = subposx + TORPEDO_OFS_X;
          torpedoy = subposy + TORPEDO_OFS_Y;
        }
      }
  
      if ((key_keystat() & down_key) != 0) {
        if (subposy < SUBM_MAX_Y)
          subposy += 4;
      } else {
        if ((key_keystat() & up_key) != 0) {
          if (subposy > SUBM_MIN_Y)
            subposy -= 4;
        }
      }
  
      if ((key_keystat() & left_key) != 0) {
        if (subposx > SUBM_MIN_X)
          subposx -= 8;
      } else {
        if ((key_keystat() & right_key) != 0) {
          if (subposx < SUBM_MAX_X)
            subposx += 4;
        }
      }
    } else {
      if (subposx > SUBM_TARGET_X)
        subposx -= 8;
      else if (subposx < SUBM_TARGET_X)
        subposx += 4;

      if (subposy > SUBM_TARGET_Y)
        subposy -= 4;
    }

    if (key_keypressed(break_key))
      if (escape(time, xpos)) {
        return false;
      }

    if (key_keypressed(pause_key))
      pause(time, xpos);

    key_readkey();

    for (b = 0; b <= fishcnt; b++) {
      if (fish[b].x >= -SPR_FISHWID) {
        fish[b].x -= 2;
        fish[b].y += fish[b].ydir;
        if (fish[b].y > 300 || fish[b].y < 80)
          fish[b].ydir = -fish[b].ydir;

        if (fish[b].state >= 32)
          fish[b].state = ((fish[b].state + 1) & 31) + 32;
        else
          fish[b].state = (fish[b].state + 1) & 31;

        if ((fish[b].state < 32) &&
            (fish[b].x > subposx - 40) &&
            (fish[b].x < subposx + 120) &&
            (fish[b].y > subposy - 40) &&
            (fish[b].y < subposy + 40)) {
          pts_add(50);
          fish[b].x = - (SPR_FISHWID + 1);
        }
      }
    }

    if (nextfish > 0)
      nextfish--;
    else {
      for (b = 0; b <= fishcnt; b++) {
        if (fish[b].x < -SPR_FISHWID) {
          fish[b].x = SCREENWID;
          fish[b].y = rand() / (RAND_MAX / 140) + 120;
          fish[b].state = 32;
          do {
            fish[b].ydir = rand() / (RAND_MAX / 10) - 5;
          } while (fish[b].ydir == 0);
          nextfish = rand() / (RAND_MAX / 20) + 5;
          break;
        }
      }
    }

//    scr_putbar(0, 0, SCREENWID, SCREENHEI, 0, 0, 0, 255);

    if (time < 300)
      towerpos = -(4*time);
    else
      towerpos = gametime * scrollerspeed - 4*time + SCREENWID + (SPR_SLICEWID*2);

    scr_draw_bonus1(xpos, towerpos);

    if (torpedox != -1)
      scr_draw_torpedo(torpedoy, torpedox);
    scr_draw_submarine(subposy - 20, subposx, time % 9);

    for (b = 0; b <= fishcnt; b++) {
      if (fish[b].x >= -SPR_FISHWID)
        scr_draw_fish(fish[b].y, fish[b].x, fish[b].state);

    }
    scr_draw_bonus2(xpos, towerpos);

    scr_swap();

    if (xpos == 600)
      scr_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());

    if (time == gametime) {
      automatic = true;
      if ((subposx == SUBM_TARGET_X) && (subposy == SUBM_TARGET_Y)) break;
    } else {
      xpos +=4;
      time++;
    }
    dcl_wait();

  } while (true);

  return true;
}
