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
#include "archi.h"
#include "palette.h"
#include "menu.h"
#include "points.h"
#include "decl.h"
#include "sound.h"
#include "bonus.h"
#include "level.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

static void printhelp(void) {
  printf("  -f enable fullscreen\n");
  printf("  -s silence, disable all sound\n");
}

static bool parse_arguments(int argc, char *argv[]) {
  for (int t = 1; t < argc; t++) {
    if (!strcmp(argv[t], "-f")) fullscreen = true;
    else if (!strcmp(argv[t], "-s")) nosound = true;
    else {
      printhelp();
      return false;
    }
  }
  return true;
}

static void startgame(void) {

  unsigned char stat, tower;
  Uint8 anglepos;
  int gameresult;
  Uint16 resttime;

  arc_init("toppler.dat");
  lev_findmissions();
  gam_init();
  men_init();
  snd_init();
  stat = men_main();
//  bns_game();
  //return;
  while (stat > 0) {
    gam_newgame();
    tower = 0;
    do {
      snd_wateron();
      do {
        gam_loadtower(tower);
        pal_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());

        pal_calcdark(pal_towergame);
        snd_watervolume(128);
        snd_playtgame();
        gam_arrival();
        gameresult = gam_towergame(anglepos, resttime);
        snd_stoptgame();
      } while ((gameresult == GAME_DIED) && pts_lifesleft());

      if (gameresult == GAME_FINISHED) {
        gam_pick_up(anglepos, resttime);

        snd_wateroff();

        if (tower < lev_towercount()) {

          // load next tower, because its colors will be needed for bonus game
          tower++;
          gam_loadtower(tower);

          if (!bns_game())
            gameresult = GAME_ABBORTED;
          return;
        }
      } else {
        snd_wateroff();
      }
    } while (pts_lifesleft() && (tower != 8) && (gameresult != GAME_ABBORTED));

    men_highscore(pts_points());
    stat = men_main();
  }
  lev_done();
  snd_done();
  gam_done();
  arc_done();
}

int main(int argc, char *argv[]) {

  load_config();

  if (parse_arguments(argc, argv)) {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("Nebulous", NULL);
    int mouse = SDL_ShowCursor(0);
    atexit(SDL_Quit);
    printf("Nebulous version "VERSION"\n");
    srand(time(0));
    startgame();
    printf("thanks for playing\n");
    SDL_ShowCursor(mouse);
    SDL_Quit();
  }
  
  save_config();

  return 0;
}
