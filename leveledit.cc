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
#include <string.h>

/* Editor key actions.
 If you add here, change _ed_key_actions[] in leveledit.cc */

typedef enum {
  EDACT_QUIT,
  EDACT_MOVEUP,
  EDACT_MOVEDOWN,
  EDACT_MOVELEFT,
  EDACT_MOVERIGHT,
  EDACT_INSROW,
  EDACT_DELROW,
  EDACT_ROT180,
  EDACT_PUTSPACE,
  EDACT_PUTSTEP,
  EDACT_PUTVANISHER,
  EDACT_PUTSLIDER,
  EDACT_PUTDOOR,
  EDACT_PUTGOAL,
  EDACT_CHECKTOWER,
  EDACT_PUTROBOT1,
  EDACT_PUTROBOT2,
  EDACT_PUTROBOT3,
  EDACT_PUTROBOT4,
  EDACT_PUTROBOT5,
  EDACT_PUTROBOT6,
  EDACT_PUTROBOT7,
  EDACT_PUTLIFT,
  EDACT_PUTLIFTMID,
  EDACT_PUTLIFTTOP,
  EDACT_PUTSTICK,
  EDACT_PUTBOX,
  EDACT_LOADTOWER,
  EDACT_SAVETOWER,
  EDACT_TESTTOWER,
  EDACT_SETTOWERCOLOR,
  EDACT_INCTIME,
  EDACT_DECTIME,
  EDACT_CREATEMISSION,
  EDACT_MOVEPAGEUP,
  EDACT_MOVEPAGEDOWN,
  EDACT_GOTOSTART,
  EDACT_SHOWKEYHELP,
  EDACT_NAMETOWER,
  EDACT_SETTIME,
  EDACT_REC_DEMO,
  EDACT_PLAY_DEMO,
  
  NUMEDITORACTIONS    
} key_actions;

struct _ed_key {
   key_actions action;
   SDLKey key;
};

#define TOWERPAGESIZE 5 /* pageup/pagedown moving */
#define TOWERSTARTHEI 4 /* tower starting height */

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
   "Go To Start",   "Show This Help", "Name The Tower",  "Set Tower time",
   "Record Demo",   "Play Demo"
};

const struct _ed_key _ed_keys[] = {
   {EDACT_QUIT,          SDLK_ESCAPE},
   {EDACT_SHOWKEYHELP,   SDLK_F1},
   {EDACT_MOVEUP,        SDLK_UP},
   {EDACT_MOVEDOWN,      SDLK_DOWN},
   {EDACT_MOVELEFT,      SDLK_LEFT},
   {EDACT_MOVERIGHT,     SDLK_RIGHT},
   {EDACT_MOVEPAGEUP,    SDLK_PAGEUP},
   {EDACT_MOVEPAGEDOWN,  SDLK_PAGEDOWN},
   {EDACT_GOTOSTART,     SDLK_HOME},
   {EDACT_ROT180,        SDLK_y},
   {EDACT_INSROW,        SDLK_INSERT},
   {EDACT_DELROW,        SDLK_DELETE},
   {EDACT_PUTSPACE,      SDLK_SPACE},
   {EDACT_PUTSTEP,       SDLK_w},
   {EDACT_PUTVANISHER,   SDLK_s},
   {EDACT_PUTSLIDER,     SDLK_x},
   {EDACT_PUTDOOR,       SDLK_i},
   {EDACT_PUTGOAL,       SDLK_k},
   {EDACT_PUTROBOT1,     SDLK_1},
   {EDACT_PUTROBOT2,     SDLK_2},
   {EDACT_PUTROBOT3,     SDLK_3},
   {EDACT_PUTROBOT4,     SDLK_4},
   {EDACT_PUTROBOT5,     SDLK_5},
   {EDACT_PUTROBOT6,     SDLK_6},
   {EDACT_PUTROBOT7,     SDLK_7},
   {EDACT_PUTLIFT,       SDLK_c},
   {EDACT_PUTLIFTMID,    SDLK_d},
   {EDACT_PUTLIFTTOP,    SDLK_e},
   {EDACT_PUTSTICK,      SDLK_q},
   {EDACT_PUTBOX,        SDLK_a},
   {EDACT_CHECKTOWER,    SDLK_z},
   {EDACT_LOADTOWER,     SDLK_l},
   {EDACT_SAVETOWER,     SDLK_o},
   {EDACT_TESTTOWER,     SDLK_p},
   {EDACT_SETTOWERCOLOR, SDLK_v},
   {EDACT_SETTIME,       SDLK_b},
   {EDACT_DECTIME,       SDLK_n},
   {EDACT_CREATEMISSION, SDLK_m},
   {EDACT_NAMETOWER,     SDLK_t},
   {EDACT_REC_DEMO,      SDLK_F10},
   {EDACT_PLAY_DEMO,     SDLK_F11}
};

static int bg_row;
static int bg_col;
static char *bg_text = NULL;

/* men_yn() background drawing callback proc */
static void editor_background_proc(void) {
   scr_drawedit(bg_row * 4, bg_col * 8, false);
   if (bg_text) scr_writetext_center(5, bg_text);
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

  int activecol = 0, abg_r = 10, abg_g = 50, abg_b = 120;
  bool ende = false, oldpal = false;
  char cbuf[32];
  int c, tmpcol, z, tmp;
  int oldc[3], newc[3], curc[3];

  const char *colorname[] = {"Red", "Green", "Blue"};

  curc[0] = newc[0] = oldc[0] = lev_towercol_red();
  curc[1] = newc[1] = oldc[1] = lev_towercol_green();
  curc[2] = newc[2] = oldc[2] = lev_towercol_blue();

  do {

    scr_drawedit(row * 4, col * 8, false);
    scr_writetext_center(22, "Tower Color");

    for (tmp = 0 ; tmp < 3; tmp++) {
      scr_color_ramp(&abg_r, &abg_g, &abg_b);
      tmpcol = newc[tmp];
      z = ((SCREENHEI * 2) / 3) + tmp * (FONTHEI + 2);

      if (tmp == activecol) {
        scr_putbar((SCREENWID / 2) - (128 + 8), z, 8, FONTHEI, abg_r, abg_g, abg_b, 255);
        scr_putbar((SCREENWID / 2) + 128 - 1, z, 8, FONTHEI, abg_r, abg_g, abg_b, 255);
      } else {
        scr_putbar((SCREENWID / 2) - (128 + 8), z, 8, FONTHEI, 66, 66, 66, 255);
        scr_putbar((SCREENWID / 2) + 128 - 1, z, 8, FONTHEI, 66, 66, 66, 255);
      }
      scr_putbar((SCREENWID / 2) - 128 + tmpcol, z, 255 - tmpcol, FONTHEI, 0,0,0, 255);
      switch (tmp) {
      default:
      case 0: scr_putbar((SCREENWID / 2) - 128, z, tmpcol, FONTHEI, tmpcol / 3 + 64, 0, 0, 255); break;
      case 1: scr_putbar((SCREENWID / 2) - 128, z, tmpcol, FONTHEI, 0, tmpcol / 3 + 64, 0, 255); break;
      case 2: scr_putbar((SCREENWID / 2) - 128, z, tmpcol, FONTHEI, 0, 0, tmpcol / 3 + 64, 255); break;
      }
      cbuf[0] = '\0';
      sprintf(cbuf, "%5s  %.3d", colorname[tmp], tmpcol);
      scr_writetext_center(z, cbuf);
    }

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
    "Exit is unreachable",
    "Not enough time",
    "Tower is too short",
    "Tower has no name"
  };

  pr = lev_is_consistent(r, c);
  if ((r >= lev_towerrows()) && (lev_towerrows() > 0)) r = lev_towerrows() - 1;
  bg_row = r;
  bg_col = -c;

  bg_text = "Tower Check:";
  men_info(problemstr[pr % NUM_TPROBLEMS], 50, 2);
  row = bg_row;
  col = bg_col;
  bg_text = NULL;
}

static void createMission(void) {

  scr_drawedit(0, 0, false);
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

    scr_drawedit(0, 0, false);
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

    scr_drawedit(0, 0, false);
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

void le_showkeyhelp(int row, int col) {
  int c, k, offs = 0;
  bool ende = false;
  int lines = ((SCREENHEI - 75) / FONTHEI);
  char pointup[2], pointdown[2];

  pointup[0] = fontptrup;
  pointup[1] = 0;
  pointdown[0] = fontptrdown;
  pointdown[1] = 0;

  do {
    (void)key_readkey();

    scr_drawedit(row * 4, col * 8, false);

    scr_writetext_center(5, "Editor Key Help");

    for (k = 0; k < lines; k++) {
      char *buf;
      int len;

      buf = SDL_GetKeyName(_ed_keys[k+offs].key);

      len = scr_textlength(buf);
      scr_writetext(185 - len, k * FONTHEI + 75, buf);
      scr_writetext(185 + FONTWID, k * FONTHEI + 75, _ed_key_actions[_ed_keys[k+offs].action]);
    }

    if (offs > 0) scr_writetext(SCREENWID-FONTWID, 34, pointup);
    if (offs + lines < SIZE(_ed_keys)) scr_writetext(SCREENWID-FONTWID, SCREENHEI-FONTHEI, pointdown);

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

  (void)key_readkey();
}

void le_edit(void) {

  bool ende = false;
  bool changed = false;
  SDLKey inp;
  int row = 0, col = 0;
  int tstep = 0;
  int blink_r = 70, blink_g = 40, blink_b = 10;
  char status[80];

  lev_new(TOWERSTARTHEI);

  inp = SDLK_UNKNOWN;

  set_men_bgproc(editor_background_proc);

  lev_set_towertime(100 + (rand() % 10) * 50);

  lev_set_towercol(rand() % 256,rand() % 256,rand() % 256);
  scr_settowercolor(lev_towercol_red(),
                    lev_towercol_green(),
                    lev_towercol_blue());

  lev_set_towername("");
  lev_set_towerdemo(0, NULL);

  while (!ende) {

    if (key_keypressed(quit_action)) break;

    bg_row = row;
    bg_col = col;

    scr_drawedit(row * 4, col * 8, true);

    status[0] = '\0';
    sprintf(status, "%c~t050X%d~t150Y%d",
            changed ? '*' : ' ', -col & 0xf, row);

    scr_putbar(SCREENWID-8, SCREENHEI-lev_towerrows(), 8, lev_towerrows(),
               lev_towercol_red(), lev_towercol_green(), lev_towercol_blue(), 255);
    scr_putbar(SCREENWID-8, SCREENHEI-row-1, 8, 1, blink_r, blink_g, blink_b, 128);

    scr_color_ramp(&blink_r, &blink_g, &blink_b);

    scr_writeformattext(0, SCREENHEI-FONTHEI, status);

    scr_swap();
    dcl_wait();

    inp = key_sdlkey();

    if (inp != SDLK_UNKNOWN) {
      int k, action = -1;

      (void)key_sdlkey();

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
        bg_text = "Load Tower:";
        men_input(editor_towername, TOWERNAMELEN);
        bg_text = NULL;
	if ((strlen(editor_towername) > 0) &&
	    lev_loadtower(editor_towername)) {
	    scr_settowercolor(lev_towercol_red(),
			      lev_towercol_green(),
			      lev_towercol_blue());
	    changed = false;
	}
        break;
      case EDACT_SAVETOWER:
        bg_text = "Save Tower:";
        men_input(editor_towername, TOWERNAMELEN);
        bg_text = NULL;
        lev_savetower(editor_towername);
        changed = false;
        break;

      case EDACT_REC_DEMO:
	{
	    Uint8 dummy1;
	    Uint16 dummy2;
	    unsigned char *p;
	    int demolen = -1;
	    Uint16 *demobuf = NULL;
	    lev_set_towerdemo(0, NULL);
	    lev_save(p);
	    gam_newgame();
	    rob_initialize();
	    snb_init();
	    snd_wateron();
	    gam_towergame(dummy1, dummy2, demolen, &demobuf);
	    snd_wateroff();
	    lev_restore(p);
	    lev_set_towerdemo(demolen, demobuf);
	    key_readkey();
	    set_men_bgproc(editor_background_proc);
	}
	break;
      case EDACT_PLAY_DEMO:
	{
	    int demolen = 0;
	    Uint16 *demobuf = NULL;
	    lev_get_towerdemo(demolen, demobuf);
	    if (demolen > 0) {
		Uint8 dummy1;
		Uint16 dummy2;
		unsigned char *p;
		lev_save(p);
		gam_newgame();
		rob_initialize();
		snb_init();
		snd_wateron();
		gam_towergame(dummy1, dummy2, demolen, &demobuf);
		snd_wateroff();
		lev_restore(p);
		key_readkey();
		set_men_bgproc(editor_background_proc);
	    } else {
		men_info("No recorded demo", 150, 2);
	    }
	}
	break;
      case EDACT_TESTTOWER:
        {
          Uint8 dummy1;
          Uint16 dummy2;
	  int dummy3 = 0;
	  Uint16 *dummybuf = NULL;
          unsigned char *p;
          lev_save(p);
          gam_newgame();
          rob_initialize();
          snb_init();
          snd_wateron();
          gam_towergame(dummy1, dummy2, dummy3, &dummybuf);
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
      case EDACT_SETTIME:
        {
          char buf[16];
          int l, p = 0, ml;

          l = lev_towertime();

          while ((l > 0) && (p < 15)) {
            buf[p] = '0' + (l % 10);
            p++;
            l = l / 10;
          }
          buf[p] = '\0';
          ml = 0;
          p = (p / 2) + 1;
          while (p > 0) {
            char tmp = buf[p];
            buf[p] = buf[ml];
            buf[ml] = tmp;
            p--;
            ml++;
          }

          bg_text = "Enter Tower Time:";
          men_input((char *)buf, 15, -1, -1, "0123456789");
          bg_text = NULL;

          lev_set_towertime(atoi(buf));
        }
        break;
      case EDACT_CREATEMISSION:
        createMission();
        break;
      case EDACT_MOVEPAGEUP:
        if (row + TOWERPAGESIZE < lev_towerrows())
          row += TOWERPAGESIZE;
        else
          row = lev_towerrows() - 1;
        break;
      case EDACT_MOVEPAGEDOWN:
        if (row > TOWERPAGESIZE)
          row -= TOWERPAGESIZE;
        else
          row = 0;
        break;
      case EDACT_GOTOSTART:
        col = 0;
        row = 1;
        break;
      case EDACT_NAMETOWER:
        bg_text = "Name the Tower:";
        men_input(lev_towername(), TOWERNAMELEN);
        bg_text = NULL;
        changed = true;
        break;
      case EDACT_SHOWKEYHELP: le_showkeyhelp(row, col); break;
      default: break;
      }
    }
  }
}
