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
#include "menu.h"
#include "decl.h"
#include "sound.h"
#include "level.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

static void printhelp(void) {
  printf("\n\tOptions:\n\n");
  printf("  -f\tEnable fullscreen mode\n");
  printf("  -s\tSilence, disable all sound\n");
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

  arc_init("toppler.dat");
  lev_findmissions();
  gam_init();
  men_init();
  snd_init();
  men_main();
  lev_done();
  snd_done();
  gam_done();
  arc_done();
}

int main(int argc, char *argv[]) {

  load_config();

#ifdef VERSION
  printf("Nebulous version " VERSION "\n");
#else
  printf("Nebulous\n");
#endif

  if (parse_arguments(argc, argv)) {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("Nebulous", NULL);
    int mouse = SDL_ShowCursor(fullscreen ? 0 : 1);
    tt_has_focus = true;
    atexit(SDL_Quit);
    srand(time(0));
    startgame();
    printf("Thanks for playing!\n");
    SDL_ShowCursor(mouse);
    SDL_Quit();
  }
  
  save_config();

  return 0;
}
