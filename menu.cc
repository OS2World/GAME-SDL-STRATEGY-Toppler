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

#include "menu.h"

#include "game.h"
#include "points.h"
#include "bonus.h"
#include "sprites.h"
#include "archi.h"
#include "screen.h"
#include "keyb.h"
#include "decl.h"
#include "level.h"
#include "sound.h"
#include "leveledit.h"

#include <SDL_endian.h>

#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static unsigned short menupicture, titledata;
static unsigned char currentmission = 0;

#define NUMHISCORES 10
#define SCORENAMELEN 9

static struct {
  unsigned int points;
  char name[SCORENAMELEN+1];
} scores[NUMHISCORES];


void men_init(void) {

  Uint8 pal[3*256];
  Uint32 res;
  Uint8 num;

  arc_assign(menudat);

  arc_read(&num, 1, &res);
  arc_read(pal, 3*num+3, &res);

  menupicture = scr_loadsprites_new(1, 640, 480, false, pal);
  arc_closefile();

  arc_assign(titledat);

  arc_read(&num, 1, &res);
  arc_read(pal, 3*num+3, &res);
  titledata = scr_loadsprites_new(1, SPR_TITLEWID, SPR_TITLEHEI, true, pal);
  arc_closefile();
}

static callback_proc menu_background_proc = NULL;

void set_men_bgproc(callback_proc proc) {
   menu_background_proc = proc;
}


  
typedef char *FDECL((*menuopt_callback_proc), (void *));

#define MENUTITLELEN  ((SCREENWID / FONTMINWID) + 1)
#define MENUOPTIONLEN MENUTITLELEN

/* Menu option flags */
#define MOFLAG_PASSKEYS 0x01  /* Do keys get passed onto this option? */

struct _menuoption {
   char oname[MENUOPTIONLEN];
   menuopt_callback_proc oproc;
   int  ostate;
   int  oflags;
   char quickkey;
};

struct _menusystem {
   char title[MENUTITLELEN];
   int numoptions;
   struct _menuoption *moption;
   menuopt_callback_proc mproc;
   int hilited;
   int mstate;
   int maxoptlen;
   bool exitmenu;
   bool wraparound;
   bool activatedoption;
   int ystart;
   char key;
};

struct _menusystem *
new_menu_system(char *title, menuopt_callback_proc pr, int molen = 0, int ystart = 25)
{
  struct _menusystem *ms = (struct _menusystem *)malloc(sizeof(struct _menusystem));

  if (ms) {
    memset(ms->title, '\0', MENUTITLELEN);
    memcpy(ms->title, title, (strlen(title) < MENUTITLELEN) ? strlen(title) + 1 : (MENUTITLELEN-1));

    ms->numoptions = 0;
    ms->moption = NULL;
    ms->mstate = 0;
    ms->mproc = pr;
    ms->maxoptlen = molen;
    ms->exitmenu = false;
    ms->wraparound = false;
    ms->activatedoption = false;
    ms->hilited = 0;
    ms->ystart = ystart;
    ms->key = 0;
  }

  return ms;
}

struct _menusystem *
add_menu_option(struct _menusystem *ms,
                char *name,
                menuopt_callback_proc pr,
                char quickkey = '\0',
                int flags = 0,
                int state = 0) {
  struct _menuoption *tmp;
  int olen = 0;

  if (!ms) return ms;

  tmp = (struct _menuoption *)malloc(sizeof(struct _menuoption)*(ms->numoptions+1));

  if (!tmp) return ms;

  memcpy(tmp, ms->moption, sizeof(struct _menuoption)*ms->numoptions);
  free(ms->moption);

  memset(tmp[ms->numoptions].oname, '\0', MENUOPTIONLEN);

  /* if no name, but has callback proc, query name from it. */
  if (!name && pr) name = (*pr) (NULL);

  if (name) {
    olen = strlen(name);
    memcpy(tmp[ms->numoptions].oname, name, (olen < MENUOPTIONLEN) ? olen + 1 : (MENUOPTIONLEN-1));
  }

  tmp[ms->numoptions].oproc = pr;
  tmp[ms->numoptions].ostate = state;
  tmp[ms->numoptions].oflags = flags;
  tmp[ms->numoptions].quickkey = quickkey;

  ms->moption = tmp;
  ms->numoptions++;
  if (name)
    olen = scr_textlength(name);
  else
    olen = 0;
  if (ms->maxoptlen < olen) ms->maxoptlen = olen;

  return ms;
}


void 
free_menu_system(struct _menusystem *ms)
{
  if (!ms) return;

  free(ms->moption);
  ms->numoptions = 0;
  ms->mstate = 0;
  ms->mproc = NULL;
  free(ms);
}


void 
draw_menu_system(struct _menusystem *ms)
{
  static Uint8 blink_color = 0;

  if (!ms) return;

  int y, offs = 0, len;
  bool has_title = strlen(ms->title);

  if (ms->wraparound) {
    if (ms->hilited < 0)
      ms->hilited = ms->numoptions - 1;
    else if (ms->hilited >= ms->numoptions)
      ms->hilited = 0;
  } else {
    if (ms->hilited < 0)
      ms->hilited = 0;
    else if (ms->hilited >= ms->numoptions)
      ms->hilited = ms->numoptions - 1;
  }

  if (ms->mproc) (*ms->mproc) (ms);

  if (has_title) scr_writetext_center(ms->ystart, ms->title);

  /* TODO: Calculate offs from ms->hilited.
   * TODO: Put slider if more options than fits in screen.
   */

  for (y = 0; (ms->ystart + ((y+1)+(has_title ? 3 : 1))*FONTHEI < SCREENHEI) && (y + offs < ms->numoptions); y++) {
    len = strlen(ms->moption[y+offs].oname);
    if (y + offs == ms->hilited)
      scr_putbar((SCREENWID - ms->maxoptlen - 8) / 2,
                 ms->ystart + (y + (has_title ? 3 : 1)) * FONTHEI - 3,
                 ms->maxoptlen + 8, FONTHEI + 3, blink_color, blink_color, blink_color, 128);
    if (len)
      scr_writetext_center(ms->ystart + (y + (has_title ? 3 : 1)) * FONTHEI, ms->moption[y+offs].oname);
  }

  blink_color = (blink_color + 5) & 0xFF;
  scr_swap();
  dcl_wait();
}

struct _menusystem *
run_menu_system(struct _menusystem *ms)
{
  if (!ms) return ms;

  do {
    draw_menu_system(ms);

    ms->key = key_chartyped();

    if (ms->key) {
      key_readkey();

      if (isalpha(ms->key))
        for (int tmpz = 0; tmpz < ms->numoptions; tmpz++)
          if (ms->moption[tmpz].quickkey == ms->key) {
            ms->hilited = tmpz;
            ms->key = ' ';
            break;
          }

      if ((ms->moption[ms->hilited].oflags & MOFLAG_PASSKEYS) &&
          (ms->moption[ms->hilited].oproc) && (ms->key != '\0')) {
        char *tmpbuf = (*ms->moption[ms->hilited].oproc) (ms);
        if (tmpbuf) {
          int olen = strlen(tmpbuf);
          memset(ms->moption[ms->hilited].oname, '\0', MENUOPTIONLEN);
          memcpy(ms->moption[ms->hilited].oname, tmpbuf,
                 (olen < MENUOPTIONLEN) ? olen + 1 : (MENUOPTIONLEN-1));

          olen = scr_textlength(tmpbuf);
          if (ms->maxoptlen < olen) ms->maxoptlen = olen;
          ms->key = '\0';
        }
      }

      switch (ms->key) {
      case 2:
        if (ms->wraparound) {
          do {
            ms->hilited = (ms->hilited + 1) % ms->numoptions;
          } while (!strlen(ms->moption[ms->hilited].oname));
        } else {
          if (ms->hilited < ms->numoptions) {
            ms->hilited++;
            if (!strlen(ms->moption[ms->hilited].oname)) ms->hilited++;
          }
        }
        break;
      case 1:
        if (ms->wraparound) {
          do {
            ms->hilited--;
            if (ms->hilited < 0) ms->hilited = ms->numoptions - 1;
          } while (!strlen(ms->moption[ms->hilited].oname));
        } else {
          if (ms->hilited > 0) {
            ms->hilited--;
            if (!strlen(ms->moption[ms->hilited].oname)) ms->hilited--;
          }
        }
        break;
      case ' ':
      case '\r': if ((ms->hilited >= 0) && (ms->hilited < ms->numoptions) &&
                     ms->moption[ms->hilited].oproc) {
        char *tmpbuf = (*ms->moption[ms->hilited].oproc) (ms);
        if (tmpbuf) {
          int olen = strlen(tmpbuf);
          memset(ms->moption[ms->hilited].oname, '\0', MENUOPTIONLEN);
          memcpy(ms->moption[ms->hilited].oname, tmpbuf,
                 (olen < MENUOPTIONLEN) ? olen + 1 : (MENUOPTIONLEN-1));
          olen = scr_textlength(tmpbuf);
          if (ms->maxoptlen < olen) ms->maxoptlen = olen;
        }
        break;
      }
      case '\e': ms->exitmenu = true; break;
      default:
        break;
      }
    }
  } while (!ms->exitmenu);
  return ms;
}

static char *
men_yn_background_proc(void *ms)
{
  if (menu_background_proc) (*menu_background_proc) ();
  return "";
}

static char *
men_yn_option_yes(void *tmp)
{
  struct _menusystem *ms = (struct _menusystem *)tmp;
  if (ms) {
    ms->mstate = 1;
    ms->exitmenu = true;
    return NULL;
  } else return "Yes";
}

static char *
men_yn_option_no(void *tmp)
{
  struct _menusystem *ms = (struct _menusystem *)tmp;
  if (ms) {
    ms->mstate = 0;
    ms->exitmenu = true;
    return NULL;
  } else return "No";
}

unsigned char men_yn(char *s, bool defchoice) {
  struct _menusystem *ms = new_menu_system(s, men_yn_background_proc, 0, SCREENHEI / 5);

  bool doquit = false;

  if (!ms) return defchoice;

  ms = add_menu_option(ms, NULL, men_yn_option_no, 'N');
  ms = add_menu_option(ms, NULL, men_yn_option_yes, 'Y');

  ms->mstate = defchoice ? 1 : 0;

  ms = run_menu_system(ms);

  doquit = ms->mstate;

  free_menu_system(ms);

  return ((doquit == 0) ? 0 : 1);
}

void men_info(char *s, long timeout, int fire) {
   bool ende = false;
  do {
     if (menu_background_proc) (*menu_background_proc) ();
     scr_writetext_center((SCREENHEI / 5), s);
     if (fire)
       scr_writetext_center((SCREENHEI / 5) + 2 * FONTHEI, (fire == 1) ? "Press fire" : "Press space");
     scr_swap();
     dcl_wait();
     if (timeout > 0) timeout--;
     if (!timeout) ende = true;
     else if ((fire == 2) && (key_chartyped() == ' ')) ende = true;
     else if ((fire != 2) && key_keypressed(fire_key)) ende = true;
  } while (!ende);
}

static char *
men_options_background_proc(void *ms)
{
  if (ms) {
    scr_blit(spr_spritedata(menupicture), 0, 0);
    scr_blit(spr_spritedata(titledata), (SCREENWID - spr_spritedata(titledata)->w) / 2, 20);
    return NULL;
  }
  return "";
}

static char *
men_options_windowed(void *ms)
{
  if (ms) {
    fullscreen = !fullscreen;
    scr_reinit();
  }
  if (fullscreen) return "Windowed";
  else return "Fullscreen";
}

static char *
men_options_scaling(void *ms)
{
  if (ms) {
//    doublescale = !doublescale;
    scr_reinit();
  }
/*  if (doublescale) return "Do not scale";
  else*/ return "Scale  2x";
}

static char *
men_options_sounds(void *ms)
{
  if (ms) {
    if (nosound) {
      nosound = false;
      snd_init();
    } else {
      snd_stoptitle();
      snd_stoptgame();
      snd_done();
      nosound = true;
    }
  }
  if (nosound) return "Sounds  On";
  else return "Sounds Off";
}

static char *
men_options(void *mainmenu) {
  static char s[20] = "Options";
  if (mainmenu) {

    struct _menusystem *ms = new_menu_system(s, men_options_background_proc, 0, spr_spritedata(titledata)->h+30);

    if (!ms) return NULL;

    ms = add_menu_option(ms, NULL, men_options_windowed);
    ms = add_menu_option(ms, NULL, men_options_scaling);
    ms = add_menu_option(ms, NULL, men_options_sounds);

    ms = add_menu_option(ms, NULL, NULL);
    ms = add_menu_option(ms, "Back", NULL);

    ms = run_menu_system(ms);

    free_menu_system(ms);
  }
  return s;
}
  
static void emptyscoretable(void) {
  for (int t = 0; t < NUMHISCORES; t++) {
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
        if (strcasecmp(mname, lev_missionname(currentmission)) == 0) {

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
        if (strcasecmp(mname, lev_missionname(currentmission)) == 0) {
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

#define HISCORES_PER_PAGE 5

static int hiscores_timer = 0;
static int hiscores_pager = 0;
static int hiscores_state = 0;
static int hiscores_xpos = SCREENWID;
static int hiscores_hilited = -1;
static int hiscores_maxlen_pos = 0;
static int hiscores_maxlen_points = 0;
static int hiscores_maxlen_name = 0;
static int hiscores_maxlen = 0;

void
get_hiscores_string(int p, char **pos, char **points, char **name)
{
  if ((p < 0) || (p >= NUMHISCORES))
    *pos = * points = *name = "";
  static char buf1[SCORENAMELEN + 5];
  static char buf2[SCORENAMELEN + 5];
  static char buf3[SCORENAMELEN + 5];
  buf1[0] = buf2[0] = buf3[0] = '\0';
  sprintf(buf1, "%i", p + 1);
  sprintf(buf2, "%i", scores[p].points);
  sprintf(buf3, "%s", scores[p].name);

  *pos = buf1;
  *points = buf2;
  *name = buf3;
}

void
calc_hiscores_maxlen(int *max_pos, int * max_points, int *max_name)
{
  for (int x = 0; x < NUMHISCORES; x++) {
    char *a, *b, *c;
    int clen;

    get_hiscores_string(x, &a, &b, &c);

    clen = scr_textlength(a);
    if (clen > *max_pos) *max_pos = clen;

    clen = scr_textlength(b);
    if (clen > *max_points) *max_points = clen;

    clen = scr_textlength(c);
    if (clen > *max_name) *max_name = clen;

  }
}

static char *
men_hiscores_background_proc(void *ms)
{
  static Uint8 blink_color = 0;

  if (ms) {

    scr_blit(spr_spritedata(menupicture), 0, 0);
    scr_blit(spr_spritedata(titledata), (SCREENWID / 2) - (SPR_TITLEWID / 2), 20);

    switch (hiscores_state) {
    case 0: /* bring the scores in */
      if (hiscores_xpos > ((SCREENWID - hiscores_maxlen) / 2)) {
        hiscores_xpos -= 10;
        break;
      } else hiscores_state = 1;
    case 1: /* hold the scores on screen */
      if (hiscores_timer < 100) {
        hiscores_timer++;
        break;
      } else hiscores_state = 2;
    case 2: /* move the scores out */
      if (hiscores_xpos > -hiscores_maxlen) {
        hiscores_timer = 0;
        hiscores_xpos -= 10;
        break;
      } else {
        hiscores_state = 0;
        hiscores_xpos = SCREENWID;
        hiscores_pager = (hiscores_pager + 1) % (NUMHISCORES / HISCORES_PER_PAGE);
      }
    default: break;
    }
    for (int t = 0; t < HISCORES_PER_PAGE; t++) {
      int cs = t + (hiscores_pager * HISCORES_PER_PAGE);
      char *pos, *points, *name;
      get_hiscores_string(cs, &pos, &points, &name);
      if (cs == hiscores_hilited) {
        int clen = hiscores_maxlen_pos + hiscores_maxlen_points + hiscores_maxlen_name + 20 + 10;
        scr_putbar(hiscores_xpos - 5, (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45 - 3, clen, FONTHEI + 3, blink_color, blink_color, blink_color, 128);
      }
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos - scr_textlength(pos), (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, pos);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 10 + hiscores_maxlen_points - scr_textlength(points) , (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, points);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 20 + hiscores_maxlen_points, (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, name);
    }
    blink_color = (blink_color + 5) & 0xFF;

    return NULL;
  }
  return "HighScores";
}

static void show_scores(bool back = true, int mark = -1) {

  struct _menusystem *ms = new_menu_system("", men_hiscores_background_proc, 0, SCREENHEI-(3*FONTHEI));

  if (!ms) return;

  getscores();

  hiscores_timer = 0;
  if ((mark >= 0) && (mark < NUMHISCORES))
    hiscores_pager = (mark / HISCORES_PER_PAGE);
  else
    hiscores_pager = 0;
  hiscores_state = 0;
  calc_hiscores_maxlen(&hiscores_maxlen_pos, &hiscores_maxlen_points, &hiscores_maxlen_name);
  hiscores_maxlen = hiscores_maxlen_pos + hiscores_maxlen_points + hiscores_maxlen_name + 20;
  hiscores_xpos = SCREENWID;
  hiscores_hilited = mark;

  if (back)
    ms = add_menu_option(ms, "Back", NULL);
  else
    ms = add_menu_option(ms, "Ok", NULL);

  ms = run_menu_system(ms);

  free_menu_system(ms);
}

void
main_game_loop()
{
  unsigned char tower = 0;
  Uint8 anglepos;
  Uint16 resttime;
  int gameresult;

  lev_loadmission(currentmission);

  gam_newgame();
  do {
    snd_wateron();
    do {
      gam_loadtower(tower);
      scr_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());
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
      }
    } else {
      snd_wateroff();
    }
  } while (pts_lifesleft() && (tower != 8) && (gameresult != GAME_ABBORTED));

  men_highscore(pts_points());
}

#ifdef HUNT_THE_FISH
static char *
men_main_bonusgame_proc(void *ms)
{
  if (ms) {
    snd_stoptitle();
    gam_newgame();
    scr_settowercolor(rand() % 256, rand() % 256, rand() % 256);
    bns_game();
    snd_playtitle();
  }
  return "Hunt the Fish";
}
#endif /* HUNT_THE_FISH */

static char *
men_main_startgame_proc(void *ms)
{
  if (ms) {
    struct _menusystem *tms = (struct _menusystem *)ms;
    int missioncount = lev_missionnumber();
    switch (tms->key) {
    case ' ':
    case '\r':
      snd_stoptitle();
      main_game_loop();
      snd_playtitle();
      break;
    case 3: currentmission = (currentmission + 1) % missioncount; break;
    case 4: currentmission = (currentmission + missioncount - 1) % missioncount; break;
    default: return NULL;
    }
  }
  static char s[30];
  sprintf(s, "Start: %s", lev_missionname(currentmission));
  return s;
}

static char *
men_main_highscore_proc(void *ms)
{
  if (ms) {
    show_scores();
  }
  return "Highscores";
}

static char *
men_main_leveleditor_proc(void *ms)
{
  if (ms) {
    snd_stoptitle();
    le_edit();
//    pal_colors(pal_menu);
    snd_playtitle();
  }
  return "Level Editor";
}

void men_main() {
  struct _menusystem *ms;

  ms = new_menu_system("", men_options_background_proc, 0, spr_spritedata(titledata)->h + 10);

  if (!ms) return;

//  pal_colors(pal_menu);

  snd_playtitle();

  ms = add_menu_option(ms, NULL, men_main_startgame_proc, '\0', MOFLAG_PASSKEYS);
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, NULL, men_main_highscore_proc, 'H');
  ms = add_menu_option(ms, NULL, men_options, 'O');
  ms = add_menu_option(ms, NULL, men_main_leveleditor_proc, 'E');
#ifdef HUNT_THE_FISH
  ms = add_menu_option(ms, NULL, men_main_bonusgame_proc);
#endif
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, "Quit", NULL, 'Q');

  ms->wraparound = true;

  ms = run_menu_system(ms);

  free_menu_system(ms);

  snd_stoptitle();
}

static int input_box_cursor_state = 0;

void
draw_input_box(int x, int y, int len, int cursor, char *txt)
{
  static int col = 0;

  scr_putbar(x, y, len * FONTMAXWID, FONTHEI, 0, 0, 0, 128);
  scr_writetext(x+1,y, txt);

  if ((input_box_cursor_state & 4) && ((cursor >= 0) && (cursor < len)))
    scr_putbar(x + scr_textlength(txt, cursor) + 1, y, FONTMINWID, FONTHEI, (fontcol + 14 - col) * 0xf, (fontcol + 14 - col) * 0xf, (14 - col) * 0xf, 128);
  scr_putrect(x,y, len * FONTMAXWID, FONTHEI, col, col, col, 128);
  input_box_cursor_state++;

  col = (col + 5) & 0xFF;
}

void men_input(char *s, int max_len, int xpos, int ypos) {
  char inp;
  char pos = 0;

  if (xpos < 0) xpos = (SCREENWID / 2) - max_len * (FONTMAXWID / 2);
  if (xpos < 0) xpos = 0;

  if (ypos < 0) ypos = (SCREENHEI / 2) - (FONTHEI / 2);

  s[0] = 0;

  do {
    if (menu_background_proc) (*menu_background_proc) ();

    draw_input_box(xpos,ypos, max_len, pos, s);
    scr_swap();
    dcl_wait();

    inp = key_chartyped();

    switch (inp) {
    case 4: if ((unsigned)pos < strlen(s)) pos++; break;
    case 3: if (pos > 0) pos--; break;
    case '\e': s[0] = '\0'; pos = 0; break;
    case '\r': break;
    case 6:
      if (strlen(s) >= (unsigned)pos) {
        int ztmp;
        for (ztmp = pos; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
        s[ztmp] = '\0';
      }
      break;
    case '\b':
      if (pos > 0) {
        if (pos <= max_len) {
          int ztmp;
          for (ztmp = pos-1; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
          s[ztmp] = '\0';
        }
        pos--;
      }
      break;
    default:
      if ((isalnum(inp) || (inp == ' ')) && (pos < max_len) &&
          (strlen(s) >= (unsigned)pos) && (strlen(s) < (unsigned)max_len)) {
        int ztmp;
        for (ztmp = max_len-1; ztmp >= pos; ztmp--) s[ztmp+1] = s[ztmp];
        s[pos] = inp;
        s[max_len] = '\0';
        pos++;
      }
      break;
    }
  } while (inp != '\r');
}


static int congrats_placement = 0;

static void
congrats_background_proc(void)
{
  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), (SCREENWID / 2) - (SPR_TITLEWID / 2), 20);

  scr_writetext_center(130, "Congratulations");
  if (congrats_placement == 0) {
    scr_writetext_center(170, "You got the");
    scr_writetext_center(210, "highest score");
  } else {
    char buf[40];
    sprintf(buf, "%i best players", NUMHISCORES);
    scr_writetext_center(170, "You are one of the");
    scr_writetext_center(210, buf);
  }
  scr_writetext_center(270, "Please enter your name");
}
  
void men_highscore(unsigned long pt) {

//  pal_colors(pal_menu);

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

  int t = NUMHISCORES;

  while ((t > 0) && (pt > scores[t-1].points)) {
    if (t < NUMHISCORES)
      scores[t] = scores[t-1];
    t--;
  }

  if (t < NUMHISCORES) {
    congrats_placement = t;
    set_men_bgproc(congrats_background_proc);

    strncpy(scores[t].name, getenv("LOGNAME"), SCORENAMELEN);
    scores[t].name[SCORENAMELEN] = 0; //to be sure

    men_input(scores[t].name, SCORENAMELEN);

    scores[t].points = pt;

    savescores();
  }

#ifdef USE_LOCKING
  unlink(SCOREFILE ".lck");
#endif

  show_scores(false, t);
}

void men_done(void) {
}

