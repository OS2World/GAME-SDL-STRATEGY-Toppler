#include "leveledit.h"

#include "decl.h"
#include "palette.h"
#include "level.h"
#include "screen.h"
#include "keyb.h"
#include "game.h"
#include "sound.h"
#include "robots.h"
#include "snowball.h"
#include "menu.h"

static bool really_quit(int row, int col) {
  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_towergame);
  scr_drawedit(row * 4, col * 8);
  scr_writetext_center(41, "TOWER CHANGED");
  scr_writetext_center(61, "REALLY QUIT");
  scr_writetext_center(95,  "ESC   YES QUIT");
  scr_writetext_center(112, "OTHER NO      ");

  scr_swap();

  int inp;

  do {
    inp = key_chartyped();
  } while (!inp);

  if (inp == 27) {
    return true;
  } else {
    pal_colors(pal_towergame);
    return false;
  }
}

static bool really_load(int row, int col) {
  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_towergame);
  scr_drawedit(row * 4, col * 8);
  scr_writetext_center(41, "TOWER CHANGED");
  scr_writetext_center(61, "REALLY LOAD");
  scr_writetext_center(95,  "ENTER  YES LOAD");
  scr_writetext_center(112, "OTHER  NO      ");

  scr_swap();

  int inp;

  do {
    inp = key_chartyped();
  } while (!inp);

  if (inp == 13) {
    return true;
  } else {
    pal_colors(pal_towergame);
    return false;
  }
}

static void no_problems(int row, int col) {
  key_readkey();
  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_towergame);
  scr_drawedit(row * 4, col * 8);
  scr_writetext_center(61, "NO PROBLEMS");
  scr_writetext_center(81, "FOUND");
  scr_writetext_center(115,  "PRESS KEY");

  scr_swap();

  int inp;

  do {
    inp = key_chartyped();
  } while (!inp);

  pal_colors(pal_towergame);
}

static void createMission(void) {

  pal_darkening(fontcol, fontcol + fontcnt - 1, pal_towergame);
  scr_drawedit(0, 0);
  scr_writetext_center(30, "MISSION CREATION");
  scr_writetext_center(60, "ENTER MISSION NAME");
  scr_writetext_center(70, "EMPTY TO ABBORT");

  char missionname[25];
  men_input(missionname, 25, 160);

  if (!missionname[0]) {
    pal_colors(pal_towergame);
    return;
  }

  if (!lev_mission_new(missionname)) {

    scr_drawedit(0, 0);
    scr_writetext_center(30, "MISSION CREATION");

    scr_writetext_center(70, "COULD NOT CREATE FILE");
    scr_writetext_center(90, "ABBORTING");

    scr_swap();

    int inp;

    do {
      inp = key_chartyped();
    } while (!inp);
  
    pal_colors(pal_towergame);

    return;
  }

  int currenttower = 1;
  char towername[30];

  while (true) {

    scr_drawedit(0, 0);
    scr_writetext_center(30, "MISSION CREATION");
    scr_writetext_center(60, "ENTER NAME OF");

    char s[30];
    sprintf(s, "TOWER NO %i", currenttower);
    scr_writetext_center(70, s);

    towername[0] = 0;
    men_input(towername, 25, 160);

    if (!towername[0]) break;

    lev_mission_addtower(towername);

    currenttower++;
  }

  lev_mission_finish();

  lev_findmissions();
}


void le_edit(void) {

  bool ende = false;
  bool changed = false;
  bool palchanged = false;
  char inp;
  int row = 0, col = 0;
  int rstep = 0, gstep = 0, bstep = 0, tstep = 0;
  char tname[20] = "tower";

//  lev_new();
  pal_settowercolor(255, 0, 0);
  pal_calcdark(pal_towergame);
  pal_colors(pal_towergame);

  key_readkey();

  while (!ende) {

    scr_drawedit(row * 4, col * 8);
    scr_swap();
    dcl_wait();

    inp = key_chartyped();

    switch (inp) {
    case 1:
      if (row + 1 < lev_towerrows()) row++;
      break;
    case 2:
      if (row > 0) row--;
      break;
    case 'Y':
      col += 8;
      break;
    case 3:
      col ++;
      break;
    case 4:
      col --;
      break;
    case 5:
      lev_insertrow(row);
      changed = true;
      break;
    case 6:
      lev_deleterow(row);
      changed = true;
      break;
    case 7:
      if (row + 5 < lev_towerrows())
        row += 5;
      else
        row = lev_towerrows() - 1;
      break;
    case 8:
      if (row > 5)
        row -= 5;
      else
        row = 0;
      break;
    case 9:
      col = 0;
      row = 1;
      break;
    case 27:
      if (changed) {
        ende = really_quit(row, col);
      } else {
        ende = true;
      }
      break;
    case ' ':
      lev_putspace(row, -col & 0xf);
      changed = true;
      break;

    case 'W':
      lev_putstep(row, -col & 0xf);
      changed = true;
      break;
    case 'S':
      lev_putvanishingstep(row, -col & 0xf);
      changed = true;
      break;
    case 'X':
      lev_putslidingstep(row, -col & 0xf);
      changed = true;
      break;

    case 'I':
      lev_putdoor(row, -col & 0xf);
      changed = true;
      break;
    case 'K':
      lev_puttarget(row, -col & 0xf);
      changed = true;
      break;

    case '1':
      lev_putrobot1(row, -col & 0xf);
      changed = true;
      break;
    case '2':
      lev_putrobot2(row, -col & 0xf);
      changed = true;
      break;
    case '3':
      lev_putrobot3(row, -col & 0xf);
      changed = true;
      break;
    case '4':               
      lev_putrobot4(row, -col & 0xf);
      changed = true;
      break;
    case '5':
      lev_putrobot5(row, -col & 0xf);
      changed = true;
      break;
    case '6':
      lev_putrobot6(row, -col & 0xf);
      changed = true;
      break;
    case '7':
      lev_putrobot7(row, -col & 0xf);
      changed = true;
      break;

    case 'E':
      lev_puttopstation(row, -col & 0xf);
      changed = true;
      break;
    case 'D':
      lev_putmiddlestation(row, -col & 0xf);
      changed = true;
      break;
    case 'C':
      lev_putelevator(row, -col & 0xf);
      changed = true;
      break;

    case 'Q':
      lev_putstick(row, -col & 0xf);
      changed = true;
      break;
    case 'A':
      lev_putbox(row, -col & 0xf);
      changed = true;
      break;
    case 'L':
      if (changed)
        if (!really_load(row, col))
          break;
      men_input(tname, 19, 160);
      lev_loadtower(tname);
      palchanged = true;
      changed = false;
      break;
    case 'O':
      men_input(tname, 19, 160);
      lev_savetower(tname);
      changed = false;
      break;
    case 'P':
      {
        int dummy1, dummy2;
        unsigned char *p;
        lev_save(p);
        gam_newgame();
        rob_initialize();
        snb_init();
        snd_wateron();
        gam_towergame(dummy1, dummy2);
        pal_colors(pal_towergame);
        snd_wateroff();
        lev_restore(p);
        key_readkey();
      }
      break;
    case 'T':
      if (rstep <= 0) rstep = 1;
      if (lev_towercol_red() + rstep > 255) {
        lev_set_towercol(255,
                         lev_towercol_green(),
                         lev_towercol_blue());
      } else {
        lev_set_towercol(lev_towercol_red() + rstep,
                         lev_towercol_green(),
                         lev_towercol_blue());

        if (rstep < 10) rstep++;
      }
      palchanged = true;
      changed = true;
      break;
    case 'R':
      if (rstep >= 0) rstep = -1;
      if (lev_towercol_red() + rstep < 0) {
        lev_set_towercol(0,
                         lev_towercol_green(),
                         lev_towercol_blue());
      } else {
        lev_set_towercol(lev_towercol_red() + rstep,
                         lev_towercol_green(),
                         lev_towercol_blue());

        if (rstep > -10) rstep--;
      }
      palchanged = true;
      changed = true;
      break;
    case 'G':
      if (gstep <= 0) gstep = 1;
      if (lev_towercol_green() + gstep > 255) {
        lev_set_towercol(lev_towercol_red(),
                         255,
                         lev_towercol_blue());
      } else {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green() + gstep,
                         lev_towercol_blue());

        if (gstep < 10) gstep++;
      }
      palchanged = true;
      changed = true;
      break;
    case 'F':
      if (gstep >= 0) gstep = -1;
      if (lev_towercol_green() + gstep < 0) {
        lev_set_towercol(lev_towercol_red(),
                         0,
                         lev_towercol_blue());
      } else {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green() + gstep,
                         lev_towercol_blue());

        if (gstep > -10) gstep--;
      }
      palchanged = true;
      changed = true;
      break;
    case 'B':
      if (bstep <= 0) bstep = 1;
      if (lev_towercol_blue() + bstep > 255) {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green(),
                         255);
      } else {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green(),
                         lev_towercol_blue() + bstep);

        if (bstep < 10) bstep++;
      }
      palchanged = true;
      changed = true;
      break;
    case 'V':
      if (bstep >= 0) bstep = -1;
      if (lev_towercol_blue() + bstep < 0) {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green(),
                         0);
      } else {
        lev_set_towercol(lev_towercol_red(),
                         lev_towercol_green(),
                         lev_towercol_blue() + bstep);

        if (bstep > -10) bstep--;
      }
      palchanged = true;
      changed = true;
      break;
    case 'H':
      if (tstep <= 0) tstep = 1;
      lev_set_towertime(lev_towertime() + tstep);
      if (tstep < 10) tstep++;
      changed = true;
      break;
    case 'N':
      if (tstep >= 0) tstep = -1;
      lev_set_towertime(lev_towertime() + tstep);
      if (tstep > -10) tstep--;
      changed = true;
      break;
    case 'Z':
      {
        int r, c;
        if (!lev_is_consistent(r, c)) {
          row = r;
          col = -c;
        } else {
          no_problems(row, col);
        }
      }
      break;
    case 'M':
      createMission();
      break;
    }
    if (palchanged) {
      pal_settowercolor(lev_towercol_red(),
                        lev_towercol_green(),
                        lev_towercol_blue());
      pal_calcdark(pal_towergame);
      pal_colors(pal_towergame);

      palchanged = false;
    }
  }
}
