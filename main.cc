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
  int anglepos;
  int gameresult;
  int resttime;

  arc_init("toppler.dat");
  lev_findmissions();
  gam_init();
  men_init();
  snd_init();
  stat = men_main(true);
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
        gam_arrival();
        gameresult = gam_towergame(anglepos, resttime);
      } while ((gameresult == GAME_DIED) && pts_lifesleft());

      if (gameresult == GAME_FINISHED) {
        gam_pick_up(anglepos, resttime);

        snd_wateroff();

        if (tower < lev_towercount()) {

          /* load next tower, because its colors will be needed for bonus game */
          tower++;
          gam_loadtower(tower);

          if (!bns_game())
            gameresult = GAME_ABBORTED;
        }
      } else {
        snd_wateroff();
      }
    } while (pts_lifesleft() && (tower != 8) && (gameresult != GAME_ABBORTED));

    men_highscore(pts_points());
    stat = men_main(false);
  }
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
