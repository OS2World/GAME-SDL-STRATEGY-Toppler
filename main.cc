/* Tower Toppler - Nebulus
 * Copyright (C) 2000-2003  Andreas Röver
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
#include "menu.h"
#include "decl.h"
#include "sound.h"
#include "level.h"
#include "configuration.h"
#include "highscore.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <stdexcept>

static void printhelp(void) {
  printf("\n\tOptions:\n\n");
  printf("  -f\tEnable fullscreen mode\n");
  printf("  -s\tSilence, disable all sound\n");
  printf("  -dX\tSet debug level to X  (default: %i)\n", config.debug_level());
}

static bool parse_arguments(int argc, char *argv[]) {
  for (int t = 1; t < argc; t++) {
    if (!strncmp(argv[t], "-f", 2)) config.fullscreen(true);
    else if (!strcmp(argv[t], "-s", 2)) config.nosound(true);
    else if (strstr(argv[t], "-d", 2) == argv[t]) {
      char parm = argv[t][2];
      if (parm >= '0' && parm <= '9') {
        printf("Debug level is now %c.\n", parm);
        config.debug_level(parm - '0');
      } else printf("Illegal debug level value, using default.\n");
    } else {
      printhelp();
      return false;
    }
  }
  return true;
}

static void startgame(void) {

  lev_findmissions();
  gam_init();
  men_init();
  snd_init();
  men_main();
  lev_done();
  snd_done();
  gam_done();
}

static void QuitFunction(void) {
  SDL_Quit();
}

int main(int argc, char *argv[]) {

#ifdef VERSION
  printf("Nebulous version " VERSION "\n");
#else
  printf("Nebulous\n");
#endif

  hsc_init();

  if (parse_arguments(argc, argv)) {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("Nebulous", NULL);

    int mouse = SDL_ShowCursor(config.fullscreen() ? 0 : 1);
    tt_has_focus = true;
    atexit(QuitFunction);
    srand(time(0));
    startgame();
    printf("Thanks for playing!\n");
    SDL_ShowCursor(mouse);
    SDL_Quit();
  }
  
  return 0;
}
