#include "menu.h"

#include "sprites.h"
#include "archi.h"
#include "screen.h"
#include "keyb.h"
#include "palette.h"
#include "decl.h"
#include "level.h"
#include "sound.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static unsigned short menupicture, titledata;
static unsigned char currentmission = 0;

#define missioncount 2

static unsigned char menupicpalette[3*240];

static struct {
  unsigned int points;
  char name[10];
} scores[10];


/* the picture is compressed using a simple lz77 version */
static SDL_Surface *decompresstitle() {
  int palpos = 0;
  for (int t = 0; t < 240; t++) {
    menupicpalette[palpos++] = arc_getbits(8);
    menupicpalette[palpos++] = arc_getbits(8);
    menupicpalette[palpos++] = arc_getbits(8);
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

void men_init() {
  arc_assign(menudat);
  menupicture = spr_savesprite(decompresstitle());
  arc_closefile();

  arc_assign(titledat);
  titledata = scr_loadsprites(1, 304, 47, fontcol, true, true);
  arc_closefile();
}

unsigned char men_main(bool fade) {

  void *oldpal;

  pal_savepal(&oldpal);

  for (int i = 0; i < 240; i++)
    pal_setpal(i, menupicpalette[3*i], menupicpalette[3*i+1], menupicpalette[3*i+2]);

  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), 8, 20);
  scr_swap();

  snd_playtitle();

  pal_colors();

  int p = 0;
  int m = currentmission;
  int palt = 1, malt = 0;
  bool ende = false;
  char s[20];
  int t = 0;
  int main;

  do {

    if ((p != palt) || (m != malt)) {

      scr_blit(spr_spritedata(menupicture), 0, 0);
      scr_blit(spr_spritedata(titledata), 8, 20);

      sprintf(s, "mission %i", m + 1);
      scr_writetext(160-6*10, 110, s);
      scr_writetext(160-6*5, 130, "start");
      scr_writetext(160-6*10, 160, "highscore");
      scr_writetext(160-6*4, 180, "quit");

      switch(p) {
        case 0: t = 110; break;
        case 1: t = 130; break;
        case 2: t = 160; break;
        case 3: t = 180; break;
      }

      scr_writetext(60, t, "*");
      scr_writetext(320-60, t, "*");

      scr_swap();

      palt = p;
      malt = m;
    }

    key_readkey();
    while (!key_keypressed(any_key)) dcl_wait();

    if (key_keypressed(up_key))
      if (p > 0) p--; else p = 3;
    if (key_keypressed(down_key))
      if (p < 3) p++; else p = 0;
    if (key_keypressed(left_key) && (p == 0))
      m = (m + 1) % missioncount;
    if (key_keypressed(right_key) && (p == 0))
      m = (m + missioncount - 1) % missioncount;

    if (key_keypressed(fire_key))
      switch(p) {
        case 1:
          ende = true;
          break;
        case 2:
          currentmission = m;
          men_highscore(-1, false);
          palt = p + 1;
          break;
        case 3:
          ende = true;
          break;
      }
  } while (!ende);

  key_readkey();

  currentmission = m;

  sprintf(s, "mission%i.dat", m + 1);
  lev_loadmission(s);

  if (p == 1)
    main = 1;
  else
    main = 0;

  pal_restorepal(oldpal);
  pal_colors();

  snd_stoptitle();

  return main;
}

static void emptyscoretable() {
  for (int t = 0; t < 10; t++) {
    scores[t].points = 0;
    scores[t].name[0] = 0;
  }
}

static void getscores() {

  FILE *f = fopen(SCOREFILE, "rb");

  if (f) {

    fseek(f, currentmission * sizeof(scores), SEEK_SET);

    if (feof(f) || (fread(scores, 1, sizeof(scores), f) != sizeof(scores)))
      emptyscoretable();
  
    fclose(f);

  } else {
    emptyscoretable();
  }
}

static void savescores() {

  FILE *f = fopen(SCOREFILE, "r+b");

  if (!f)
    f = fopen(SCOREFILE, "a+b");

  fseek(f, currentmission * sizeof(scores), SEEK_SET);

  fwrite(scores, sizeof(scores), 1, f);

  fclose(f);
}

void men_highscore(long pt, bool pal) {

  void *oldpal;
  int lockfd;

  scr_writetext(160-6*16, 90,"SCOREFILE LOCKED");
  scr_writetext(160-6*11, 110,"PLEASE WAIT");
  scr_swap();

  if (pt > 0){ 
    while (lockfd = open(SCOREFILE ".lck", O_CREAT | O_RDWR | O_EXCL) == -1) {
      sleep(1);
      scr_swap();
    }
    close(lockfd);
  }

  if (pal) {
    pal_savepal(&oldpal);
    pal_black();
    for (int i = 0; i < 240; i++)
      pal_setpal(i, menupicpalette[3*i], menupicpalette[3*i+1], menupicpalette[3*i+2]);
    pal_colors();
  }

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
    scr_writetext(160-6*15, 70,"CONGRATULATIONS");
    if (t == 0) {
      scr_writetext(160-6*18, 90,"YOU GOT THE");
      scr_writetext(160-6*15,110,"HIGH SCORE");
    }
    else {
      scr_writetext(160-6*18, 90,"YOU ARE ONE OF THE");
      scr_writetext(160-6*15,110,"10 BEST PLAYERS");
    }
    scr_writetext(160-6*22,140,"PLEASE ENTER YOUR NAME");

    scr_putbar(100, 160, 120, 16);

    scr_swap();

    if (pal) pal_colors();
    char inp;
    char pos = 0;
    int defName = 1;
    scores[t].name[pos] = 0;

    strncpy(scores[t].name,getenv("LOGNAME"),8);
    scores[t].name[8]=0; //to be sure

    scr_putbar(100, 160, 120, 16);
    scr_writetext(160 - 6 * strlen(scores[t].name), 160, scores[t].name);
    scr_swap();

    key_readkey();

    do {

      while ((inp = key_chartyped()) == 0) dcl_wait();

      if (defName) {
	defName=0;
#if 0
	if (inp==' ') inp='\b'; /* Is this right? */
#endif
	if (inp!='\r') scores[t].name[pos] = 0;
      }

      if (inp == '\b') {
	if (pos > 0) {
          pos--;
          scores[t].name[pos] = 0;
	} //else do nothing!
      } else if ((inp != '\r') && (pos < 9)) {
        scores[t].name[pos] = inp;
        pos++;
        scores[t].name[pos] = 0;
      }

      scr_putbar(100, 160, 120, 16);
      scr_writetext(160 - 6 * pos, 160, scores[t].name);
      scr_swap();

    } while (inp != '\r');

    scores[t].points = pt;
    savescores();

    scr_blit(spr_spritedata(menupicture), 0, 0);
    scr_blit(spr_spritedata(titledata), 8, 0);
  }

  if (pt > 0) unlink(SCOREFILE ".lck");

  char s[30];

  for (int t = 0; t < 9; t++) {
    sprintf(s, "%5i %s", scores[t].points, scores[t].name);
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

  if (pal) pal_colors();

  key_readkey();
  while (!key_keypressed(any_key));
  key_readkey();

  if (pal) pal_restorepal(oldpal);
}

void men_done() {
}



