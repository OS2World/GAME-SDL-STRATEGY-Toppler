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

#include "game.h"

#include "stars.h"
#include "points.h"
#include "decl.h"
#include "screen.h"
#include "keyb.h"
#include "menu.h"
#include "level.h"
#include "elevators.h"
#include "robots.h"
#include "toppler.h"
#include "snowball.h"
#include "sound.h"

#include <string.h>

#define STATE_PLAYING 0
#define STATE_ABORTED 1
#define STATE_DIED 2
#define STATE_TIMEOUT 3
#define STATE_FINISHED 4

void gam_init(void) {
  scr_init();
  key_init();
}

void gam_done(void) {
  key_done();
  scr_done();
}

void gam_newgame(void) {
  pts_reset();
}

void gam_loadtower(Uint8 tow) {
  lev_selecttower(tow);
}

void gam_arrival(void) {
  int b, toppler, delay = 0;

  rob_initialize();
  snb_init();

  bool svisible = true;
  int substart = 0;
  int subshape = 0;

  b = 5;
  toppler = 1;

  snd_start();
  top_hide();

  key_readkey();

  do {
    scr_drawall(8, 0, lev_towertime(), svisible, subshape, substart);
       scr_writetext_center((SCREENHEI / 5), "You are entering the");
    if (strlen(lev_towername()))
       scr_writetext_center((SCREENHEI / 5) + 2 * FONTHEI, lev_towername());
    else
       scr_writetext_center((SCREENHEI / 5) + 2 * FONTHEI, "Nameless Tower");
    scr_swap();
    snd_play();

    switch (b) {

    case 5:
      snd_start();
      b = 6;
      delay = 0;
      break;

    case 6:
      delay++;
      if (delay == 10) {
        b = 0;
        snd_sub_raise();
      }
      break;

    case 0:
      substart += 2;
      if (substart == 70) {
        b = 1;
        top_show(0, toppler, 4);
        delay = 0;
      }
      break;

    case 1:
      delay++;
      subshape = 9 + delay;
      if (delay == 21)
        b = 2;
      break;

    case 2:
      toppler++;
      top_show(0, toppler, 4);
      if (toppler == 8)
        b = 3;
      break;

    case 3:
      subshape--;
      if (subshape == 9) {
        b = 4;
        snd_sub_down();
      }
      break;

    case 4:
      substart -= 2;
      if (substart == 0) {
        b = 5;
        svisible = false;
      }
      break;
    }
    dcl_wait();
  } while (!((b == 5) || key_keypressed(fire_key)));

  svisible = false;
}

void gam_pick_up(Uint8 anglepos, Uint16 time) {
  /* the shapes of the toppler when it turns after leaving a door*/
  static unsigned char door4[4] = { 0xa, 0x9, 0x8, 0x0 };

  int toppler, b, u, delay = 0;

  b = u = 0;
  toppler = 8;

  top_show(door4[0], toppler, anglepos);

  bool svisible = false;
  int subshape = 0;
  int substart = 0;

  key_readkey();

  do {
    scr_drawall(8, (4 - anglepos) & 0x7f, time, svisible, subshape, substart);
    scr_swap();

    switch (b) {

    case 0:
      u++;
      top_show(door4[u / 2], toppler, anglepos);
      if (u == 6) {
        b = 1;
        svisible = true;
      }
      break;

    case 1:
      substart += 2;
      if (substart == 70) {
        delay = 0;
        b = 2;
      }
      break;

    case 2:
      delay++;
      subshape = 9 + delay;
      if (delay == 21)
        b = 3;
      break;

    case 3:
      toppler--;
      top_show(door4[u / 2], toppler, anglepos);
      if (toppler == 1)
        b = 4;
      break;

    case 4:
      subshape--;
      if (subshape == 9) {
        b = 5;
        top_hide();
      }
      break;

    case 5:
      substart -= 2;
      if (substart == 0) {
        b = 6;
        svisible = false;
      }
      break;
    }
    dcl_wait();
  } while (!((b == 6) | key_keypressed(fire_key)));

  top_hide();
  svisible = false;
}

/* checks the new height reached and adds points */
static void new_height(int verticalpos, int &reached_height) {

  if (verticalpos <= reached_height)
    return;

  while (reached_height < verticalpos) {
    pts_add(10);
    reached_height++;
  }
}

/* updates the position of the tower on screen
 with respect to the position of the animal

 there is a slight lowpass in the vertical movement
 of the tower */
static unsigned short towerpos(int verticalpos, int &tower_position, int anglepos, int &tower_angle) {
  int i, j;

  j = anglepos - tower_angle;
  if (j > 100) j -= 0x80;
  if (j < -100) j += 0x80;
  tower_angle = anglepos;


  i = verticalpos - tower_position;

  if (i > 0)
    i = (i + 3) / 4;
  else
    i = -((3 - i) / 4);

  sts_move(j, i);
  tower_position += i;

  snd_watervolume(verticalpos > 100 ? 30 : 128 - verticalpos);

  return tower_position;
}

static int bg_tower_pos = 0;
static int bg_tower_angle = 0;
static int bg_time = 0;

/* men_yn() background drawing callback proc */
static void game_background_proc(void) {
  scr_drawall(towerpos(top_verticalpos(), bg_tower_pos,
                       top_anglepos(), bg_tower_angle), (4 - top_anglepos()) & 0x7f, bg_time, false, 0, 0);
}

static void timeout(int &tower_position, int &tower_anglepos) {

  bg_tower_pos = tower_position;
  bg_tower_angle = tower_anglepos;
  bg_time = 0;
   
  men_info("Time over", 150);
}

static void writebonus(int &tower_position, int tower_anglepos, int zeit, int tec, int extra, int time) {
  char s[30];

  scr_drawall(towerpos(top_verticalpos(), tower_position,
                       top_anglepos(), tower_anglepos), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);

  sprintf(s, "Time:      10 X %3d", zeit);
  scr_writetext_center((SCREENHEI / 2) - FONTHEI * 3, s);
  sprintf(s, "Technique: 10 X %3d", tec);
  scr_writetext_center((SCREENHEI / 2), s);
  sprintf(s, "Extra:     10 X %3d", extra);
  scr_writetext_center((SCREENHEI / 2) + FONTHEI * 3, s);

  scr_swap();
}

static void countdown(int &s) {
  if (s >= 100) {
    s -= 100;
    pts_add(1000);
    return;
  }
  if (s >= 10) {
    s -= 10;
    pts_add(100);
    return;
  }
  if (s >= 1) {
    (s)--;
    pts_add(10);
    return;
  }
}

static void bonus(int &tower_position, int &tower_angle, int time) {
  int zeit, tec, extra, delay = 0;

  zeit = time / 10;
  extra = 100;
  tec = top_technic();

  do {
     writebonus(tower_position, tower_angle, zeit, tec, extra, time);
     dcl_wait();
  } while ((delay++ < 80) && (!key_keypressed(fire_key)));

  while (zeit > 0) {
    dcl_wait();
    countdown(zeit);
    snd_score();
    snd_play();
    writebonus(tower_position, tower_angle, zeit, tec, extra, time);
  }

  while (tec > 0) {
    dcl_wait();
    countdown(tec);
    snd_score();
    snd_play();
    writebonus(tower_position, tower_angle, zeit, tec, extra, time);
  }

  while (extra > 0) {
    dcl_wait();
    countdown(extra);
    snd_score();
    snd_play();
    writebonus(tower_position, tower_angle, zeit, tec, extra, time);
  }
   
  delay = 0;

  do {
     writebonus(tower_position, tower_angle, zeit, tec, extra, time);
     dcl_wait();
  } while ((delay++ < 30) && (!key_keypressed(fire_key)));
}

/* update the time */
static void akt_time(int &time, int &timecount, Uint8 &state) {
  if (timecount >= 0) {
    timecount++;
    if (timecount == 5) {
      timecount = 0;
      time--;
      if (time <= 20 || (time <= 40 && (time % 2)))
        snd_alarm();
      if (time == 0)
        state = STATE_TIMEOUT;
    }
  }
}

static void get_keys(Sint8 &left_right, Sint8 &up_down, bool &space) {
#ifdef GAME_DEBUG_KEYS
  if ((key_keystat() & left_key) && (key_keystat() & right_key) && 
      (key_keystat() & up_key) && (key_keystat() & down_key)) {
      run_debug_menu();
  }
#endif /* GAME_DEBUG_KEYS */
  if (key_keystat() & left_key)
    left_right = -1;
  else {
    if (key_keystat() & right_key)
      left_right = 1;
    else
      left_right = 0;
  }

  if (key_keystat() & up_key)
    up_down = 1;
  else {
    if (key_keystat() & down_key)
      up_down = -1;
    else
      up_down = 0;
  }

  if (key_keypressed(fire_key))
    space = true;
  else
    space = false;
}

static void escape(Uint8 &state, int &tower_position, int &tower_anglepos, int time) {

  snd_wateroff();

  if (men_yn("Really quit", false))
    state = STATE_ABORTED;

  snd_wateron();
  towerpos(top_verticalpos(), tower_position,
           top_anglepos(), tower_anglepos);
}

static void pause(int &tower_position, int tower_anglepos, int time) {

  snd_wateroff();

  bg_tower_pos = tower_position;
  bg_tower_angle = tower_anglepos;
  bg_time = time;
   
  men_info("Pause", -1, 1);

  snd_wateron();
  towerpos(top_verticalpos(), tower_position,
           top_anglepos(), tower_anglepos);
}

gam_result gam_towergame(Uint8 &anglepos, Uint16 &resttime) {

  static Uint8 door3[6] = {
    0x17, 0x18, 0x18, 0x19, 0x19, 0xb
  };

  Sint8 left_right, up_down;
  bool space;

  Uint8 state = STATE_PLAYING;

  /* the maximal reached height for this tower */
  int reached_height;

  /* the tower position, the angle is the same as the toppler pos */
  int tower_position;
  int tower_angle;

  /* subcounter for timer */
  int timecount = 0;

  /* time left for the player to reach the tower */
  int time = lev_towertime();

  set_men_bgproc(game_background_proc);

  top_init();

  reached_height = tower_position = top_verticalpos();
  tower_angle = top_anglepos();

  ele_init();
  key_readkey();

  do {

    get_keys(left_right, up_down, space);

    bg_tower_pos = tower_position;
    bg_tower_angle = tower_angle;
    bg_time = time;

    if (key_keypressed(quit_action)) {
      state = STATE_ABORTED;
      break;
    }

    if (key_keypressed(break_key))
      escape(state, tower_position, tower_angle, time);

    if (key_keypressed(pause_key))
      pause(tower_position, tower_angle, time);

    key_readkey();

    ele_update();
    snb_movesnowball();
    top_aktualtoppler(left_right, up_down, space);

    if (!top_dying())
      rob_new(top_verticalpos());

    rob_aktualize();
    top_testkollision();

    akt_time(time, timecount, state);
    new_height(top_verticalpos(), reached_height);
    scr_drawall(towerpos(top_verticalpos(), tower_position,
                         top_anglepos(), tower_angle), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);
    scr_swap();
    snd_play();
    dcl_wait();
  } while (!top_ended() && (state == STATE_PLAYING));

  if (top_targetreached()) {
    bonus(tower_position, tower_angle, time);
    rob_disappearall();

    for (int i = 0; i < 6; i++) {
      top_show(door3[i], top_verticalpos(), top_anglepos());

      rob_aktualize();
      scr_drawall(towerpos(top_verticalpos(), tower_position,
                           top_anglepos(), tower_angle), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);
      scr_swap();
      dcl_wait();
    }

    while (tower_position > 8) {

      if (top_verticalpos() > 8) {
        lev_removelayer(top_verticalpos() / 4 - 2);
        snd_crumble();
        top_drop1layer();
      }

      rob_aktualize();
      scr_drawall(towerpos(top_verticalpos(), tower_position,
                           top_anglepos(), tower_angle), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);
      scr_swap();
      snd_play();
      dcl_wait();
    }

    state = STATE_FINISHED;
  } else if (top_died()) state = STATE_DIED;

  anglepos = top_anglepos();
  resttime = time;
  key_readkey();

  switch (state) {

  case STATE_TIMEOUT:
    timeout(tower_position, tower_angle);
    pts_died();
    return GAME_DIED;

  case STATE_ABORTED:
    return GAME_ABORTED;

  case STATE_FINISHED:
    return GAME_FINISHED;

  case STATE_DIED:
    pts_died();
    return GAME_DIED;

  default:
    return GAME_FINISHED;
  }
}

