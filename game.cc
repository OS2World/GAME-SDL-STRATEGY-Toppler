#include "game.h"

#include "stars.h"
#include "points.h"
#include "decl.h"
#include "palette.h"
#include "screen.h"
#include "keyb.h"
#include "level.h"
#include "elevators.h"
#include "robots.h"
#include "toppler.h"
#include "snowball.h"
#include "sound.h"

#include <string.h>

#define STATE_PLAYING 0
#define STATE_ABBORTED 1
#define STATE_DIED 2
#define STATE_TIMEOUT 3
#define STATE_FINISHED 4

void gam_init() {
  scr_init();
  key_init();
}

void gam_done() {
  key_done();
  scr_done();
}

void gam_newgame() {
  pts_reset();
}

void gam_loadtower(int tow) {
  lev_selecttower(tow);
}

void gam_arrival() {
  int b, toppler, delay;

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

  scr_drawall(8, 0, lev_towertime(), svisible, subshape, substart);
  scr_writetext(40L, 40L, "you are entering the");
  scr_writetext(160 - strlen(lev_towername()) * 6L, 70L, lev_towername());
  scr_swap();
  pal_colors();

  do {
    scr_drawall(8, 0, lev_towertime(), svisible, subshape, substart);
    scr_writetext(40L, 40L, "you are entering the");
    scr_writetext(160 - strlen(lev_towername()) * 6L, 70L, lev_towername());
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
      if (delay == 30) {
        b = 0;
        snd_sub_raise();
      }
      break;

    case 0:
      substart++;
      if (substart == 28) {
        b = 1;
        top_show(0, toppler, 4);
      }
      break;

    case 1:
      subshape++;
      if (subshape == 20)
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
      if (subshape == 0) {
        b = 4;
        snd_sub_down();
      }
      break;

    case 4:
      substart--;
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

void gam_pick_up(int anglepos, int time) {
  /* the shapes of the toppler when it turns after leaving a door*/
  static unsigned char door4[4] = { 0xa, 0x9, 0x8, 0x0 };

  int toppler, b, u;

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
      substart++;
      if (substart == 28)
        b = 2;
      break;

    case 2:
      subshape++;
      if (subshape == 20)
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
      if (subshape == 0) {
        b = 5;
        top_hide();
      }
      break;

    case 5:
      substart--;
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

/* checks the new height reached and add points */
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

 there is a sligt lowpass in the vertical movement
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


static void timeout(int &tower_position, int &tower_anglepos) {
  int t;

  scr_drawall(towerpos(top_verticalpos(), tower_position,
                       top_anglepos(), tower_anglepos), (4 - top_anglepos()) & 0x7f, 0, false, 0, 0);
  scr_writetext(52L, 70L, "HEY, TIME IS OVER !");
  scr_swap();
  t = 90;
  do {
    dcl_wait();
    t--;
  } while (!(key_keypressed(fire_key) || t == 0));
}

static void writebonus(int &tower_position, int tower_anglepos, int zeit, int tec, int extra, int time) {
  char s[30];

  scr_drawall(towerpos(top_verticalpos(), tower_position,
                       top_anglepos(), tower_anglepos), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);

  sprintf(s, "TIME: 10 X %d", zeit);
  scr_writetext(70L, 80L, s);
  sprintf(s, "TECHNIQUE: 10 X %d", tec);
  scr_writetext(52L, 120L, s);
  sprintf(s, "EXTRA: 10 X %d", extra);
  scr_writetext(64L, 160L, s);

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
  int zeit, tec, extra;

  zeit = time / 10;
  extra = 100;
  tec = top_technic();

  writebonus(tower_position, tower_angle, zeit, tec, extra, time);

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
}

/* update the time */
static void akt_time(int &time, int &timecount, int &state) {
  if (timecount >= 0) {
    timecount++;
    if (timecount == 5) {
      timecount = 0;
      time--;
      if ((time <= 50) && (time & 1) || (time <= 25))
        snd_alarm();
      if (time == 0)
        state = STATE_TIMEOUT;
    }
  }
}

static void get_keys(int &left_right, int &up_down, bool &space) {
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

static void escape(int &state, int &tower_position, int &tower_anglepos, int time) {
  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1);
  scr_drawall(towerpos(top_verticalpos(), tower_position,
                       top_anglepos(), tower_anglepos), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);
  scr_writetext(160 - 11 * 6,  61, "REALLY QUIT");
  scr_writetext(160 - 16 * 6,  95, "  ESC: YES, QUIT");
  scr_writetext(160 - 16 * 6, 112, "OTHER: NO PLAY");

  snd_wateroff();

  scr_swap();
  do {
  } while (!key_keypressed(any_key));
  if (key_keypressed(break_key)) {
    state = STATE_ABBORTED;
  }else
    pal_colors();

  snd_wateron();
  towerpos(top_verticalpos(), tower_position,
           top_anglepos(), tower_anglepos);
}

static void pause(int &tower_position, int tower_anglepos, int time) {
  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1);
  scr_drawall(towerpos(top_verticalpos(), tower_position,
                       top_anglepos(), tower_anglepos), (4 - top_anglepos()) & 0x7f, time, false, 0, 0);
  scr_writetext(160 -  5 * 6, 61, "PAUSE");
  scr_writetext(160 - 11 * 6, 95, "PRESS SPACE");

  snd_wateroff();

  scr_swap();
  do {
    dcl_wait();
  } while (!key_keypressed(fire_key));
  pal_colors();

  snd_wateron();
  towerpos(top_verticalpos(), tower_position,
           top_anglepos(), tower_anglepos);
}

int gam_towergame(int &anglepos, int &resttime) {

  static unsigned char door3[6] = {
    0x17, 0x18, 0x18, 0x19, 0x19, 0xb
  };

  int left_right, up_down;
  bool space;

  int state = STATE_PLAYING;

  /* the maximal reached height for this tower */
  int reached_height;

  /* the tower position, the angle is the same as the toppler pos */
  int tower_position;
  int tower_angle;

  /* subcounter for timer */
  int timecount = 0;

  /* time left for the player to reach the tower */
  int time = lev_towertime();
  
//      return GAME_FINISHED;
  top_init();

  reached_height = tower_position = top_verticalpos();
  tower_angle = top_anglepos();

  ele_init();
  key_readkey();

  do {
    get_keys(left_right, up_down, space);

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

    while (tower_position != 8) {

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

    case STATE_ABBORTED:
      return GAME_ABBORTED;
  
    case STATE_FINISHED:
      return GAME_FINISHED;
  
    case STATE_DIED:
      pts_died();
      return GAME_DIED;

    default:
      return GAME_FINISHED;
  }
}

