#include "menu.h"

#include "sprites.h"
#include "archi.h"
#include "screen.h"
#include "keyb.h"
#include "palette.h"
#include "decl.h"
#include "level.h"
#include "sound.h"
#include "leveledit.h"

#include <SDL_endian.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static unsigned short menupicture, titledata;
static unsigned char currentmission = 0;

static struct {
  unsigned int points;
  char name[10];
} scores[10];


/* the picture is compressed using a simple lz77 version */
static SDL_Surface *decompresstitle(void) {
  for (int t = 0; t < 240; t++) {
    int r = arc_getbits(8);
    int g = arc_getbits(8);
    int b = arc_getbits(8);
    pal_setpal(t, r, g, b, pal_menu);
  }

  SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 8, 0, 0, 0, 0);
  pal_setstdpalette(s);

  int bufferpos = 0;
  int copycnt, copysrc;
  unsigned char *p = (unsigned char *)s->pixels;

  do {
    if (!arc_getbits(1)) {
      p[bufferpos++] = arc_getbits(8);
    } else {
      copycnt = 3 + arc_getbits(4);
      copysrc = bufferpos - arc_getbits(12);

      while (copycnt-- > 0) p[bufferpos++] = p[copysrc++];
    }
  } while (bufferpos < 320 *240);

  return s;
}

void men_init(void) {
  arc_assign(menudat);
  menupicture = spr_savesprite(decompresstitle());
  arc_closefile();

  arc_assign(titledat);
  titledata = scr_loadsprites(1, 304, 47, fontcol, true, true);
  arc_closefile();
}

static void men_options(void) {

  int p = 0, palt, t;
  bool ende = false;
  int c;

  c = key_chartyped();
  palt = p + 1;
  do {

    if (p != palt) {

      scr_blit(spr_spritedata(menupicture), 0, 0);
      scr_blit(spr_spritedata(titledata), 8, 20);
      
      if (fullscreen)
        scr_writetext_center(130, "windowed");
      else
        scr_writetext_center(130, "fullscreen");

      if (doublescale)
        scr_writetext_center(150, "do not scale");
      else
        scr_writetext_center(150, "scale 2x");

      scr_writetext_center(190, "main");

      switch(p) {
        case 0: t = 130; break;
        case 1: t = 150; break;
        case 2: t = 190; break;
      }

      scr_writetext_center(t, "*                   *");
      scr_swap();

      palt = p;
    }

    do {
      c = key_chartyped();
    } while (!c);

    if (c == 1)
      if (p > 0) p--; else p = 2;
    if (c == 2)
      if (p < 2) p++; else p = 0;
    if (((c == 3) || (c == 4)) && (p == 0)) {
      fullscreen = !fullscreen;
      scr_reinit();
      palt = p + 1;
    }
    if (((c == 3) || (c == 4)) && (p == 1)) {
      doublescale = !doublescale;
      scr_reinit();
      palt = p + 1;
    }

    if (c == ' ')
      switch(p) {
        case 0:
          fullscreen = !fullscreen;
          scr_reinit();
          palt = p + 1;
          break;
        case 1:
          doublescale = !doublescale;
          scr_reinit();
          palt = p + 1;
          break;
        case 2:
          ende = true;
          break;
      }

  } while (!ende);

  key_readkey();
}

static void emptyscoretable(void) {
  for (int t = 0; t < 10; t++) {
    scores[t].points = 0;
    scores[t].name[0] = 0;
  }
}

static void savescores(void) {

#ifdef USE_LOCKING
  FILE *f = create_highscore_file(SCOREFILE);
#else
  FILE *f = create_highscore_file("toppler.hsc");
#endif

  if (f) {
    unsigned char len;
    char mname[30];

    while (!feof(f)) {

      if ((fread(&len, 1, 1, f) == 1) &&
          (fread(mname, 1, len, f) == len)) {
        mname[len] = 0;
        if (strcmp(mname, lev_missionname(currentmission)) == 0) {

          // this is necessary because some system can not switch
          // on the fly from reading to writing
          fseek(f, ftell(f), SEEK_SET);

          fwrite(scores, sizeof(scores), 1, f);
          fclose(f);
          return;
        }
      } else
        break;

      fseek(f, ftell(f) + sizeof(scores), SEEK_SET);
    }

    unsigned char tmp = strlen(lev_missionname(currentmission));

    fwrite(&tmp, 1, 1, f);
    fwrite(lev_missionname(currentmission), 1, tmp, f);
    fwrite(scores, sizeof(scores), 1, f);
    fclose(f);
  }
}

static void getscores(void) {

#ifdef USE_LOCKING
  FILE *f = open_highscore_file(SCOREFILE);
#else
  FILE *f = open_highscore_file("toppler.hsc");
#endif

  if (f) {

    unsigned char len;
    char mname[30];

    while (true) {

      if ((fread(&len, 1, 1, f) == 1) &&
          (fread(mname, 1, len, f) == len) &&
          (fread(scores, 1, sizeof(scores), f) == sizeof(scores))) {
        mname[len] = 0;
        if (strcmp(mname, lev_missionname(currentmission)) == 0) {
          break;
        }
      } else {
        emptyscoretable();
        break;
      }

    }

    fclose(f);

  } else {
    emptyscoretable();
  }
}

static void show_scores(int mark = 10) {
  char s[100];
  
  getscores();

  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), 8, 0);
  
  for (int t = 0; t < 9; t++) {
    if (t == mark)
      sprintf(s, "*%5i %s", scores[t].points, scores[t].name);
    else
      sprintf(s, " %5i %s", scores[t].points, scores[t].name);
    scr_writetext(10, 52 + t * 17, s);
  }

  sprintf(s, "MISSION %i", currentmission + 1);

  char sx[2];
  sx[1] = 0;

  for (int t = 0; t < 9; t++) {
    sx[0] = s[t];
    scr_writetext(220 + 50 - 15 + 30 * t / 8, 60 + t * 10, sx);
  }

  scr_writetext(270-6*5,190,"PRESS");
  scr_writetext(270-6*5,210,"SPACE");

  scr_swap();

  key_readkey();
  while (!key_keypressed(any_key));
  key_readkey();
}


unsigned char men_main() {

  pal_colors(pal_menu);
  
  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), 8, 20);
  scr_swap();

  snd_playtitle();

  int p = 0;
  int m = currentmission;
  int palt = 1, malt = 0;
  bool ende = false;
  char s[20];
  int t = 0;
  int main;
  int missioncount = lev_missionnumber();

  int c = key_chartyped();

  do {

    if ((p != palt) || (m != malt)) {

      scr_blit(spr_spritedata(menupicture), 0, 0);
      scr_blit(spr_spritedata(titledata), 8, 20);

      sprintf(s, "start %s", lev_missionname(m));
      scr_writetext_center(100, s);
      scr_writetext_center(130, "highscore");
      scr_writetext_center(150, "options");
      scr_writetext_center(170, "level editor");
      scr_writetext_center(200, "quit");
      

      switch(p) {
        case 0: t = 100; break;
        case 1: t = 130; break;
        case 2: t = 150; break;
        case 3: t = 170; break;
        case 4: t = 200; break;
      }

      scr_writetext_center(t, "*                   *");
      scr_swap();

      palt = p;
      malt = m;
    }

    do {
      c = key_chartyped();
    } while (!c);

    if (c == 1)
      if (p > 0) p--; else p = 4;
    if (c == 2)
      if (p < 4) p++; else p = 0;
    if ((c == 3) && (p == 0))
      m = (m + 1) % missioncount;
    if ((c == 4) && (p == 0))
      m = (m + missioncount - 1) % missioncount;

    if (c == ' ')
      switch(p) {
        case 0:
          currentmission = m;
          lev_loadmission(m);
          ende = true;
          break;
        case 1:
          currentmission = m;
          show_scores();
          palt = p + 1;
          break;
        case 2:
          men_options();
          palt = p + 1;
          break;
        case 3:
          le_edit();
          pal_colors(pal_menu);
          palt = p + 1;
          break;
        case 4:
          ende = true;
          break;
      }
  } while (!ende);

  key_readkey();

  if (p == 0)
    main = 1;
  else
    main = 0;

  snd_stoptitle();

  return main;
}

void men_input(char *s, int max_len, int ypos) {
  char inp;
  char pos = 0;
  int defName = 1;
  int color = 0;

  scr_putbar(160 - max_len * 6, ypos, max_len * 12, 16);
  scr_writetext_center(ypos, s);
  scr_swap();

  key_readkey();

  do {
    dcl_wait();

    inp = key_chartyped();

    if (inp) {
  
      if (inp == '\r') break;
  
      if (defName) {
        defName = 0;
        s[pos] = 0;
      }
  
      if (inp == '\b') {
        if (pos > 0) {
          pos--;
          s[pos] = 0;
        }
      } else if (pos < max_len) {
        s[pos] = inp;
        pos++;
        s[pos] = 0;
      }
    }

    scr_putbar(160 - max_len * 6, ypos, max_len * 12, 16, 16 + color);
    color = (color + 1) & 0x0f;
    scr_writetext_center(ypos, s);
    scr_swap();

  } while (inp != '\r');
}

void men_highscore(unsigned long pt) {

  pal_colors(pal_menu);

#ifdef USE_LOCKING
  int lockfd;

  scr_writetext_center(90,"SCOREFILE LOCKED");
  scr_writetext_center(110,"PLEASE WAIT");
  scr_swap();

  while ((lockfd = open(SCOREFILE ".lck", O_CREAT | O_RDWR | O_EXCL)) == -1) {
    dcl_wait();
    scr_swap();
  }
  close(lockfd);
#endif

  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), 8, 0);

  getscores();

  int t = 10;

  while ((t > 0) && (pt > scores[t-1].points)) {
    if (t < 10)
      scores[t] = scores[t-1];
    t--;
  }

  if (t < 10) {
    scr_writetext_center(70, "CONGRATULATIONS");
    if (t == 0) {
      scr_writetext_center(90, "YOU GOT THE");
      scr_writetext_center(110, "HIGHEST SCORE");
    } else {
      scr_writetext_center(90, "YOU ARE ONE OF THE");
      scr_writetext_center(110, "10 BEST PLAYERS");
    }
    scr_writetext_center(140, "PLEASE ENTER YOUR NAME");

    strncpy(scores[t].name, getenv("LOGNAME"), 9);
    scores[t].name[9] = 0; //to be sure

    men_input(scores[t].name, 9, 160);
    scores[t].points = pt;

    savescores();
  }

#ifdef USE_LOCKING
  unlink(SCOREFILE ".lck");
#endif

  show_scores(t);
}

void men_done(void) {
}



