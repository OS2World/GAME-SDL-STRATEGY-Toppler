#include "game.h"
#include "archi.h"
#include "palette.h"
#include "menu.h"
#include "points.h"
#include "decl.h"
#include "sound.h"
#include "bonus.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

static void printhelp() {
  printf("  -f enable fullscreen\n");
  printf("  -s silence, disable all sound\n");
}

static bool parse_arguments(int argc, char *argv[]) {
  fullscreen = false;
  nosound = false;

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

static void startgame() {

  /* the different colors for the eight towers */
  struct {
    unsigned char r, g, b;
  } tcolors[8] = {
    { 255, 0, 0 },
    { 150, 150, 255 },
    { 170, 170, 170 },
    { 163, 120, 88 },
    { 0, 155, 155 },
    { 255, 100, 100 },
    { 150, 255, 150 },
    { 255, 155, 0 }
  };

  unsigned char stat, tower;
  int anglepos;
  int gameresult;
  int resttime;

  arc_init(DATADIR"/nebulous.dat");
  gam_init();
  men_init();
  stat = men_main(true);
  while (stat > 0) {
    pts_reset();
    gam_newgame();
    tower = 0;

    do {
      pal_settowercolor(tcolors[tower].r, tcolors[tower].g, tcolors[tower].b);
      pal_calcdark();
      do {
        gam_loadtower(tower);
        snd_wateron();
        snd_watervolume(128);
        gam_arrival();
        gameresult = gam_towergame(anglepos, resttime);
        if (gameresult != GAME_FINISHED)
          snd_wateroff();
      } while ((gameresult == GAME_DIED) && pts_lifesleft());
      if (gameresult == GAME_FINISHED) {
        gam_pick_up(anglepos, resttime);

        snd_wateroff();

        if (tower < 7)
          if (!bns_game(tcolors[tower+1].r, tcolors[tower+1].g, tcolors[tower+1].b))
            gameresult = GAME_ABBORTED;

        tower++;
      }
    } while (pts_lifesleft() && (tower != 8) && (gameresult != GAME_ABBORTED));

    men_highscore(pts_points(), true);
    stat = men_main(false);
  }
  gam_done();
  arc_done();
}


int main(int argc, char *argv[]) {
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

  return 0;
}

