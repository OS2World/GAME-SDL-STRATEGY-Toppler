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

#include "leveledit.h"

#include "decl.h"
#include "level.h"
#include "screen.h"
#include "keyb.h"
#include "game.h"
#include "sound.h"
#include "robots.h"
#include "snowball.h"
#include "menu.h"

#include <stdlib.h>

static int bg_row;
static int bg_col;

/* men_yn() background drawing callback proc */
static void editor_background_proc(void) {
   scr_drawedit(bg_row * 4, bg_col * 8);
}

static bool really_quit(int row, int col) {

  if (men_yn("Tower changed, really quit", false)) {
    return true;
  } else {
    return false;
  }
}

static bool really_load(int row, int col) {
   
  if (men_yn("Tower changed, really load", false)) {   
    return true;
  } else {
    return false;
  }
}

static bool edit_towercolor(int row, int col) {

  int activecol = 0, bgcol = 0;
  bool ende = false, oldpal = false;
  char cbuf[32];
  int c, tmpcol, z, tmp;
  int oldc[3], newc[3], curc[3];

  const char *colorname[] = {"Red", "Green", "Blue"};

  curc[0] = newc[0] = oldc[0] = lev_towercol_red();
  curc[1] = newc[1] = oldc[1] = lev_towercol_green();
  curc[2] = newc[2] = oldc[2] = lev_towercol_blue();

  do {

    scr_drawedit(row * 4, col * 8);
    scr_writetext_center(22, "Tower Color");

    for (tmp = 0 ; tmp < 3; tmp++) {
      tmpcol = newc[tmp];
      z = ((SCREENHEI * 2) / 3) + tmp * (FONTHEI + 2);
       
      scr_putbar((SCREENWID / 2) - (128 + 8), z, 8, FONTHEI, (tmp == activecol) ? bgcol : 66, 0, 0, 255);
      scr_putbar((SCREENWID / 2) + 128 - 1, z, 8, FONTHEI, 0, (tmp == activecol) ? bgcol : 66, 0, 255);
      scr_putbar((SCREENWID / 2) - 128 + tmpcol, z, 255 - tmpcol, FONTHEI, 0,0,0, 255);
      scr_putbar((SCREENWID / 2) - 128, z, tmpcol, FONTHEI, 0, 0, brickcol + 3, 255);
      cbuf[0] = '\0';
      sprintf(cbuf, "%5s  %.3d", colorname[tmp], tmpcol);
      scr_writetext_center(z, cbuf);
    }

    bgcol = (bgcol + 5) & 0xFF;

    scr_swap();
    dcl_wait();

    c = key_chartyped();

    switch (c) {
    case 1: if (activecol > 0) activecol--; break;
    case 2: if (activecol < 2) activecol++; break;
    case 3: if (newc[activecol] > 0)   newc[activecol]--; break;
    case 4: if (newc[activecol] < 255) newc[activecol]++; break;
    case 8: if (newc[activecol] > 10)  newc[activecol] = newc[activecol] - 10;
    else newc[activecol] = 0;  break;
    case 7: if (newc[activecol] < 245)  newc[activecol] = newc[activecol] + 10;
    else newc[activecol] = 255; break;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8':
    case '9': newc[activecol] = (int) ((c - '0') * 256) / 10; break;
    case 'r':
    case 'R': activecol = 0; break;
    case 'g':
    case 'G': activecol = 1; break;
    case 'b':
    case 'B': activecol = 2; break;
    case 27: ende = true; oldpal = true; break;
    case ' ':
    case '\r': ende = true;
    default: break;
    }

    if ((newc[0] != curc[0]) || (newc[1] != curc[1]) ||
        (newc[2] != curc[2]) || oldpal) {
      if (oldpal)
        lev_set_towercol(oldc[0],oldc[1],oldc[2]);
      else
        lev_set_towercol(newc[0],newc[1],newc[2]);

      scr_settowercolor(lev_towercol_red(),
                        lev_towercol_green(),
                        lev_towercol_blue());

      curc[0] = newc[0];
      curc[1] = newc[1];
      curc[2] = newc[2];
    }

  } while (!ende);

  return (!oldpal && ((curc[0] != oldc[0]) || (curc[1] != oldc[1]) || (curc[2] != oldc[2])));
}

static void edit_checktower(int &row, int &col) {
   int r, c, pr;
   r = row;
   c = -col;
   
   static char *problemstr[NUM_TPROBLEMS] = {
      "No problems found",
      "No starting step",
      "Start is blocked",
      "Unknown block",
      "No elevator stop",
      "Elevator is blocked",
      "No opposing doorway",
      "Broken doorway",
      "No exit",
      "Exit is unreachable"
   };

   pr = lev_is_consistent(r, c);
   if ((r >= lev_towerrows()) && (lev_towerrows() > 0)) r = lev_towerrows() - 1;
   bg_row = r;
   bg_col = -c;

   men_info(problemstr[pr % NUM_TPROBLEMS], 50, 2);
   row = bg_row;
   col = bg_col;
}

static void createMission(void) {

  scr_drawedit(0, 0);
  scr_writetext_center(30, "Mission creation");
  scr_writetext_center(70, "enter mission name");
  scr_writetext_center(85, "empty to abort");

  set_men_bgproc(NULL);

  char missionname[25];
  missionname[0] = 0;
  men_input(missionname, 15);

  if (!missionname[0])
    return;

  if (!lev_mission_new(missionname)) {

    scr_drawedit(0, 0);
    scr_writetext_center(30, "Mission creation");
                              
    scr_writetext_center(70, "could not create file");
    scr_writetext_center(85, "aborting");

    scr_swap();

    int inp;

    do {
      inp = key_chartyped();
    } while (!inp);
  
    return;
  }

  int currenttower = 1;
  char towername[30];

  while (true) {

    scr_drawedit(0, 0);
    scr_writetext_center(30, "Mission creationg");
    scr_writetext_center(70, "enter name of");

    char s[30];
    sprintf(s, "tower no %i", currenttower);
    scr_writetext_center(85, s);

    towername[0] = 0;
    men_input(towername, 25);

    if (!towername[0]) break;

    lev_mission_addtower(towername);

    currenttower++;
  }

  lev_mission_finish();

  lev_findmissions();
}


#define EDACT_QUIT      0
#define EDACT_MOVEUP    1
#define EDACT_MOVEDOWN  2
#define EDACT_MOVELEFT  3
#define EDACT_MOVERIGHT 4
#define EDACT_INSROW    5
#define EDACT_DELROW    6
#define EDACT_ROT180    7
#define EDACT_PUTSPACE  8
#define EDACT_PUTSTEP   9
#define EDACT_PUTVANISHER 10
#define EDACT_PUTSLIDER   11
#define EDACT_PUTDOOR     12
#define EDACT_PUTGOAL     13
#define EDACT_CHECKTOWER  14
#define EDACT_PUTROBOT1   15
#define EDACT_PUTROBOT2   16
#define EDACT_PUTROBOT3   17
#define EDACT_PUTROBOT4   18
#define EDACT_PUTROBOT5   19
#define EDACT_PUTROBOT6   20
#define EDACT_PUTROBOT7   21
#define EDACT_PUTLIFT     22
#define EDACT_PUTLIFTMID  23
#define EDACT_PUTLIFTTOP  24
#define EDACT_PUTSTICK    25
#define EDACT_PUTBOX      26
#define EDACT_LOADTOWER   27
#define EDACT_SAVETOWER   28
#define EDACT_TESTTOWER   29
#define EDACT_SETTOWERCOLOR 30
#define EDACT_INCTIME       31
#define EDACT_DECTIME       32
#define EDACT_CREATEMISSION 33
#define EDACT_MOVEPAGEUP    34
#define EDACT_MOVEPAGEDOWN  35
#define EDACT_GOTOSTART     36
#define EDACT_SHOWKEYHELP   37
#define EDACT_NAMETOWER     38

#define NUMEDITORACTIONS  39

const char *_ed_key_actions[NUMEDITORACTIONS] = {
   "Quit",          "Move up",        "Move down",       "Move left",
   "Move right",    "Insert Row",     "Delete Row",      "Rotate 180",
   "Put Space",     "Put Step",       "Put Vanisher",    "Put Slider",
   "Put Door",      "Put Goal",       "Check Tower",     "Put Robot 1",  
   "Put Robot 2",   "Put Robot 3",    "Put Robot 4",     "Put Robot 5",
   "Put Robot 6",   "Put Robot 7",    "Put Lift",        "Lift Middle stop",
   "Lift Top stop", "Put Stick",      "Put Box",         "Load Tower",   
   "Save Tower",    "Test Tower",     "Set Tower Color", "Increase Time", 
   "Decrease Time", "Create Mission", "Move Page Up",    "Move Page Down",  
   "Go To Start",   "Show This Help", "Name The Tower"
};

struct _ed_key {
   int action;
   char key;
} _ed_keys[] = {
   {EDACT_QUIT,          27},
   {EDACT_SHOWKEYHELP,   'h'},
   {EDACT_MOVEUP,        1},
   {EDACT_MOVEDOWN,      2},
   {EDACT_MOVELEFT,      3},
   {EDACT_MOVERIGHT,     4},
   {EDACT_MOVEPAGEUP,    7},
   {EDACT_MOVEPAGEDOWN,  8},
   {EDACT_GOTOSTART,     9},
   {EDACT_ROT180,        'y'},
   {EDACT_INSROW,        5},
   {EDACT_DELROW,        6},
   {EDACT_PUTSPACE,      ' '},
   {EDACT_PUTSTEP,       'w'},
   {EDACT_PUTVANISHER,   's'},
   {EDACT_PUTSLIDER,     'x'},
   {EDACT_PUTDOOR,       'i'},
   {EDACT_PUTGOAL,       'k'},
   {EDACT_PUTROBOT1,     '1'},
   {EDACT_PUTROBOT2,     '2'},
   {EDACT_PUTROBOT3,     '3'},
   {EDACT_PUTROBOT4,     '4'},
   {EDACT_PUTROBOT5,     '5'},
   {EDACT_PUTROBOT6,     '6'},
   {EDACT_PUTROBOT7,     '7'},
   {EDACT_PUTLIFT,       'c'},
   {EDACT_PUTLIFTMID,    'd'},
   {EDACT_PUTLIFTTOP,    'e'},
   {EDACT_PUTSTICK,      'q'},
   {EDACT_PUTBOX,        'a'},
   {EDACT_CHECKTOWER,    'z'},
   {EDACT_LOADTOWER,     'l'},
   {EDACT_SAVETOWER,     'o'},
   {EDACT_TESTTOWER,     'p'},
   {EDACT_SETTOWERCOLOR, 'v'},
   {EDACT_INCTIME,       'b'},
   {EDACT_DECTIME,       'n'},
   {EDACT_CREATEMISSION, 'm'},
   {EDACT_NAMETOWER,     't'}
};

void le_showkeyhelp(int row, int col) {
  int c, k, offs = 0;
  bool ende = false;
  int lines = ((SCREENHEI - 50) / FONTHEI);
  char point[2];

  point[0] = fontpoint;
  point[1] = 0;

  do {
    scr_drawedit(row * 4, col * 8);

    scr_writetext_center(22, "Editor Key Help");

    for (k = 0; k < lines; k++) {
      char buf[80];
      int len;
      buf[0] = '\0';

      len = scr_textlength(key_name(_ed_keys[k+offs].key));

      scr_writetext(75 - len, k * FONTHEI + 50, key_name(_ed_keys[k+offs].key));
      scr_writetext(75 + FONTWID, k * FONTHEI + 50, _ed_key_actions[_ed_keys[k+offs].action]);
    }

    if (offs > 0) scr_writetext(SCREENWID-FONTWID, 34, point);
    if (offs + lines < SIZE(_ed_keys)) scr_writetext(SCREENWID-FONTWID, SCREENHEI-FONTHEI, point);

    scr_swap();
    dcl_wait();

    c = key_chartyped();

    switch (c) {
    case 1:
      if (offs > 0) offs--;
      break;
    case 2:
      if (offs + lines < SIZE(_ed_keys)) offs++;
      break;
    case 7:
      if (offs > lines) offs -= lines;
      else offs = 0;
      break;
    case ' ':
    case 8:
      if (offs + (lines * 2) < SIZE(_ed_keys)) offs += lines;
      else offs = SIZE(_ed_keys) - lines;
      break;
    case '\r':
    case 27:
      ende = true;
      break;
    }

  } while (!ende);
}

void le_edit(void) {

  bool ende = false;
  bool changed = false;
  char inp;
  int row = 0, col = 0;
  int tstep = 0;
  Uint8 blink_color = 0;

  lev_new();

  inp = 0;

  set_men_bgproc(editor_background_proc);

  lev_set_towercol(rand() % 256,rand() % 256,rand() % 256);
  scr_settowercolor(lev_towercol_red(),
                    lev_towercol_green(),
                    lev_towercol_blue());

  while (!ende) {
    bg_row = row;
    bg_col = col;

    scr_drawedit(row * 4, col * 8);

    char status[80];

    status[0] = '\0';
    sprintf(status, "%c  X%d  Y%d",
            changed ? '*' : ' ', -col & 0xf, row);

    scr_putbar(SCREENWID-8, SCREENHEI-lev_towerrows(), 8, lev_towerrows(),
               lev_towercol_red(), lev_towercol_green(), lev_towercol_blue(), 255);
    scr_putbar(SCREENWID-8, SCREENHEI-row-1, 8, 1, blink_color, blink_color, blink_color, 128);

    blink_color = (blink_color + 5) & 0xFF;

    scr_writetext(0, SCREENHEI-FONTHEI, status);

    scr_swap();
    dcl_wait();

    inp = key_chartyped();

    if (inp) {
      int k, action = -1;

      for (k = 0; k < SIZE(_ed_keys); k++)
        if (_ed_keys[k].key == inp) {
          action = _ed_keys[k].action;
          break;
        }

      switch (action) {
      case EDACT_QUIT:
        if (changed) ende = really_quit(row, col);
        else ende = true;
        break;
      case EDACT_MOVEUP:
        if (row + 1 < lev_towerrows()) row++;
        break;
      case EDACT_MOVEDOWN:
        if (row > 0) row--;
        break;
      case EDACT_MOVELEFT:
        col++;
        break;
      case EDACT_MOVERIGHT:
        col--;
        break;
      case EDACT_ROT180:
        col += 8;
        break;
      case EDACT_INSROW:
        lev_insertrow(row);
        changed = true;
        break;
      case EDACT_DELROW:
        lev_deleterow(row);
        changed = true;
        break;
      case EDACT_PUTSPACE:
        lev_putspace(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTSTEP:
        lev_putstep(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTVANISHER:
        lev_putvanishingstep(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTSLIDER:
        lev_putslidingstep(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTDOOR:
        lev_putdoor(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTGOAL:
        lev_puttarget(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_CHECKTOWER:
        edit_checktower(row, col);
        break;
      case EDACT_PUTROBOT1:
        lev_putrobot1(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT2:
        lev_putrobot2(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT3:
        lev_putrobot3(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT4:
        lev_putrobot4(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT5:
        lev_putrobot5(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT6:
        lev_putrobot6(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTROBOT7:
        lev_putrobot7(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTLIFTTOP:
        lev_puttopstation(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTLIFTMID:
        lev_putmiddlestation(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTLIFT:
        lev_putelevator(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTSTICK:
        lev_putstick(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_PUTBOX:
        lev_putbox(row, -col & 0xf);
        changed = true;
        break;
      case EDACT_LOADTOWER:
        if (changed)
          if (!really_load(row, col))
            break;
        men_input(editor_towername, TOWERNAMELEN);
        lev_loadtower(editor_towername);
        scr_settowercolor(lev_towercol_red(),
                          lev_towercol_green(),
                          lev_towercol_blue());
        changed = false;
        break;
      case EDACT_SAVETOWER:
        men_input(editor_towername, TOWERNAMELEN);
        lev_savetower(editor_towername);
        changed = false;
        break;
      case EDACT_TESTTOWER:
        {
          Uint8 dummy1;
          Uint16 dummy2;
          unsigned char *p;
          lev_save(p);
          gam_newgame();
          rob_initialize();
          snb_init();
          snd_wateron();
          gam_towergame(dummy1, dummy2);
          snd_wateroff();
          lev_restore(p);
          key_readkey();
          set_men_bgproc(editor_background_proc);
        }
        break;
      case EDACT_SETTOWERCOLOR:
        changed |= edit_towercolor(row, col);
        break;
      case EDACT_INCTIME:
        if (tstep <= 0) tstep = 1;
        lev_set_towertime(lev_towertime() + tstep);
        if (tstep < 10) tstep++;
        changed = true;
        break;
      case EDACT_DECTIME:
        if (tstep >= 0) tstep = -1;
        lev_set_towertime(lev_towertime() + tstep);
        if (tstep > -10) tstep--;
        changed = true;
        break;
      case EDACT_CREATEMISSION:
        createMission();
        break;
      case EDACT_MOVEPAGEUP:
        if (row + 5 < lev_towerrows())
          row += 5;
        else
          row = lev_towerrows() - 1;
        break;
      case EDACT_MOVEPAGEDOWN:
        if (row > 5)
          row -= 5;
        else
          row = 0;
        break;
      case EDACT_GOTOSTART:
        col = 0;
        row = 1;
        break;
      case EDACT_NAMETOWER:
        men_input(lev_towername(), TOWERNAMELEN);
        changed = true;
        break;
      case EDACT_SHOWKEYHELP: le_showkeyhelp(row, col); break;
      default: break;
      }
    }
  }
}
