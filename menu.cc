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

#define NUMHISCORES 10
#define SCORENAMELEN 9
#define HISCORES_PER_PAGE 5

#define MENUTITLELEN  ((SCREENWID / FONTMINWID) + 1)
#define MENUOPTIONLEN MENUTITLELEN

/* Menu option flags */
#define MOFLAG_PASSKEYS 0x01  /* Do keys get passed onto this option? */

/* menu option */
struct _menuoption {
   char oname[MENUOPTIONLEN];    /* text shown to user */
   menuopt_callback_proc oproc;  /* callback proc, supplies actions and the name */
   int  ostate;                  /* callback proc can use this */
   int  oflags;                  /* MOFLAG_foo */
   SDLKey quickkey;              /* quick jump key; if user presses this key,
                                  * this menu option is hilited.
                                  */
};

struct _menusystem {
   char title[MENUTITLELEN];     /* title of the menu */
   int numoptions;               /* # of options in this menu */
   struct _menuoption *moption;  /* the options */
   menuopt_callback_proc mproc;  
   int hilited;                  /* # of the option that is hilited */
   int mstate;                   /* menu state, free to use by callback procs */
   int maxoptlen;                /* longest option name length, in pixels.
                                  * the hilite bar is slightly longer than this
                                  */
   bool exitmenu;                /* if true, the menu exits */
   bool wraparound;              /* if true, the hilite bar wraps around */
   bool activatedoption;         
   int ystart;                   /* y pos where this menu begins, in pixels */
   int yhilitpos;                /* y pos of the hilite bar, in pixels */
   int opt_steal_control;        /* if >= 0, that option automagically gets
                                  * keys passed to it, and normal key/mouse
                                  * processing doesn't happen.
                                  */
   SDLKey key;                   /* the key that was last pressed */
};

static unsigned short menupicture, titledata;
static unsigned char currentmission = 0;

static struct {
  Uint32 points;
  char name[SCORENAMELEN+1];
} scores[NUMHISCORES];

static menubg_callback_proc menu_background_proc = NULL;

void set_men_bgproc(menubg_callback_proc proc) {
   menu_background_proc = proc;
}

void men_init(void) {

  Uint8 pal[3*256];

  arc_assign(menudat);

  scr_read_palette(pal);
  menupicture = scr_loadsprites_new(1, 640, 480, false, pal);
  arc_closefile();

  arc_assign(titledat);

  scr_read_palette(pal);
  titledata = scr_loadsprites_new(1, SPR_TITLEWID, SPR_TITLEHEI, true, pal);
  arc_closefile();
}

static char *
men_yn_background_proc(void *ms)
{
  if (menu_background_proc) (*menu_background_proc) ();
  return "";
}

struct _menusystem *
new_menu_system(char *title, menuopt_callback_proc pr, int molen, int ystart)
{
  struct _menusystem *ms = (struct _menusystem *)malloc(sizeof(struct _menusystem));

  if (ms) {
    memset(ms->title, '\0', MENUTITLELEN);
    if (title) {
      memcpy(ms->title, title, (strlen(title) < MENUTITLELEN) ? strlen(title) + 1 : (MENUTITLELEN-1));
    }

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
    ms->key = SDLK_UNKNOWN;
    ms->yhilitpos = ms->opt_steal_control = -1;
  }

  return ms;
}

struct _menusystem *
add_menu_option(struct _menusystem *ms,
                char *name,
                menuopt_callback_proc pr,
                SDLKey quickkey,
                int flags,
                int state) {
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
draw_menu_system(struct _menusystem *ms, Uint16 dx, Uint16 dy)
{
  static int color_r = 0, color_g = 20, color_b = 70;

  if (!ms) return;

  int y, offs = 0, len, realy, minx, miny, maxx, maxy, scrlen,
    newhilite = -1, yz, titlehei;
  bool has_title = (ms->title) && (strlen(ms->title) != 0);

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
  titlehei = has_title ? 2 : 0;

  /* TODO: Calculate offs from ms->hilited.
   * TODO: Put slider if more options than fits in screen.
   */

  yz = ms->ystart + (titlehei) * FONTHEI;

  for (y = 0; (yz+y+1 < SCREENHEI) && (y+offs < ms->numoptions); y++) {
    realy = yz + y * FONTHEI;
    len = strlen(ms->moption[y+offs].oname);
    scrlen = scr_textlength(ms->moption[y+offs].oname, len);
    minx = (SCREENWID - scrlen) / 2;
    miny = realy;
    maxx = (SCREENWID + scrlen) / 2;
    maxy = realy + FONTHEI;
    if (len) {
      if (dx >= minx && dx <= maxx && dy >= miny && dy <= maxy)
        newhilite = y + offs;
      if (y + offs == ms->hilited) {
        if (ms->yhilitpos == -1) {
          ms->yhilitpos = miny;
        } else {
          if (ms->yhilitpos < miny) ms->yhilitpos += (miny - ms->yhilitpos) / 4;
          else if (ms->yhilitpos > miny) ms->yhilitpos -= (ms->yhilitpos - miny) / 4;
        }
        scr_putbar((SCREENWID - ms->maxoptlen - 8) / 2, ms->yhilitpos - 3,
                   ms->maxoptlen + 8, FONTHEI + 3,
                   color_r, color_g, color_b, 128);
      }
    }
  }

  maxy = y;

  for (y = 0; y < maxy; y++) {
    if (strlen(ms->moption[y+offs].oname)) {
      miny = ms->ystart + (y + titlehei)*FONTHEI;
      scr_writetext_center(miny, ms->moption[y+offs].oname);
    }
  }

  if (newhilite >= 0) ms->hilited = newhilite;

  scr_color_ramp(&color_r, &color_g, &color_b);

  scr_swap();
  dcl_wait();
}


void
menu_system_caller(struct _menusystem *ms)
{
  char *tmpbuf = (*ms->moption[ms->hilited].oproc) (ms);
  if (tmpbuf) {
    int olen = strlen(tmpbuf);
    memset(ms->moption[ms->hilited].oname, '\0', MENUOPTIONLEN);
    memcpy(ms->moption[ms->hilited].oname, tmpbuf,
           (olen < MENUOPTIONLEN) ? olen + 1 : (MENUOPTIONLEN-1));
    olen = scr_textlength(tmpbuf);
    if (ms->maxoptlen < olen) ms->maxoptlen = olen;
    ms->key = SDLK_UNKNOWN;
  }
}


struct _menusystem *
run_menu_system(struct _menusystem *ms)
{
  Uint16 x,y, bttn;
  bool stolen = false;

  if (!ms) return ms;

  (void)key_sdlkey();

  do {

    stolen = false;
    bttn = false;
    x = y = 0;

    ms->key = SDLK_UNKNOWN;

    if (key_keypressed(quit_action)) {
      ms->exitmenu = true;
      break;
    }

    if ((ms->opt_steal_control >= 0) &&
        (ms->opt_steal_control < ms->numoptions) &&
        ms->moption[ms->opt_steal_control].oproc) {
      ms->key = key_sdlkey();
      ms->hilited = ms->opt_steal_control;
      stolen = true;
    } else {
      if (!fullscreen && !key_mouse(&x, &y, &bttn) && bttn)
        ms->key = key_conv2sdlkey(bttn, false);
      else ms->key = key_sdlkey();
    }

    draw_menu_system(ms, x, y);

    if ((ms->key != SDLK_UNKNOWN) || stolen) {

      if (!stolen)
        for (int tmpz = 0; tmpz < ms->numoptions; tmpz++)
          if (ms->moption[tmpz].quickkey == ms->key) {
            ms->hilited = tmpz;
            ms->key = SDLK_UNKNOWN;
            break;
          }

      if ((((ms->moption[ms->hilited].oflags & MOFLAG_PASSKEYS)) || stolen) &&
          (ms->moption[ms->hilited].oproc) && ((ms->key != SDLK_UNKNOWN) || stolen)) {
        menu_system_caller(ms);
      }
      if (!stolen) {
        switch (key_sdlkey2conv(ms->key, false)) {
        case down_key:
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
        case up_key:
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
        case fire_key:
          if ((ms->hilited >= 0) && (ms->hilited < ms->numoptions) &&
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
        case break_key : ms->exitmenu = true; break;
        default:
          break;
        }
      }
    }
  } while (!ms->exitmenu);
  return ms;
}

#ifdef GAME_DEBUG_KEYS
static char *debug_menu_extralife(void *ms) {
    if (ms) lives_add();
    return "Extra Life";
}

static char *debug_menu_extrascore(void *ms) {
    if (ms) pts_add(200);
    return "+200 Points";
}

void run_debug_menu(void) {
  struct _menusystem *ms = new_menu_system("DEBUG MENU", men_yn_background_proc, 0, SCREENHEI / 5);

  ms = add_menu_option(ms, NULL, debug_menu_extralife);
  ms = add_menu_option(ms, NULL, debug_menu_extrascore);
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, "Back to Game", NULL);

  ms = run_menu_system(ms);

  free_menu_system(ms);
}
#endif /* GAME_DEBUG_KEYS */

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

  ms = add_menu_option(ms, NULL, men_yn_option_no, SDLK_n);
  ms = add_menu_option(ms, NULL, men_yn_option_yes, SDLK_y);

  ms->mstate = defchoice ? 1 : 0;

  ms = run_menu_system(ms);

  doquit = (ms->mstate != 0);

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
  (void)key_sdlkey();
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

#define REDEFINEREC 5
static int times_called = 0;
static char *redefine_menu_up(void *tms) {
  struct _menusystem *ms = (struct _menusystem *)tms;
  static char buf[50];
  const char *code[REDEFINEREC] = {"Up", "Down", "Left", "Right", "Fire"};
  char *keystr;
  static int blink, times_called;
  const Uint16 key[REDEFINEREC] = {1, 2, 4, 8, 16};
  buf[0] = '\0';
  if (ms) {
    switch (ms->mstate) {
    default:
      if (key_sdlkey2conv(ms->key, false) == fire_key) {
        ms->mstate = 1;
        ms->opt_steal_control = ms->hilited;
        ms->key = SDLK_UNKNOWN;
      }
      break;
    case 1:
      if (ms->key != SDLK_UNKNOWN) {
        key_redefine(key[ms->hilited % REDEFINEREC], ms->key);
        ms->mstate = 2;
        ms->opt_steal_control = ms->hilited;
        blink = 0;
      } else blink++;
      break;
    case 2:
      ms->mstate = 0;
      ms->opt_steal_control = -1;
      break;
    }
    if ((blink & 4) || (ms->mstate != 1))
      keystr = SDL_GetKeyName(key_conv2sdlkey(key[ms->hilited % REDEFINEREC], true));
    else keystr = "????";
    sprintf(buf, "%s: %s", code[ms->hilited % REDEFINEREC], keystr);
  } else {
    keystr = SDL_GetKeyName(key_conv2sdlkey(key[times_called], true));
    sprintf(buf, "%s: %s", code[times_called], keystr);
    times_called = (times_called + 1) % REDEFINEREC;
  }
  return buf;
}

static char *run_redefine_menu(void *prevmenu) {
  if (prevmenu) {
    struct _menusystem *ms = new_menu_system("Redefine Keys", men_options_background_proc, 0, spr_spritedata(titledata)->h+30);

    times_called = 0;

    ms = add_menu_option(ms, NULL, redefine_menu_up);
    ms = add_menu_option(ms, NULL, redefine_menu_up);
    ms = add_menu_option(ms, NULL, redefine_menu_up);
    ms = add_menu_option(ms, NULL, redefine_menu_up);
    ms = add_menu_option(ms, NULL, redefine_menu_up);
    ms = add_menu_option(ms, "Back", NULL);

    ms = run_menu_system(ms);

    free_menu_system(ms);
  }
  return "Redefine Keys";
}

static char *
men_options_windowed(void *ms)
{
  if (ms) {
    fullscreen = !fullscreen;
    scr_reinit();
    SDL_ShowCursor(fullscreen ? 0 : 1);
  }
  if (fullscreen) return "Windowed";
  else return "Fullscreen";
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

      
    ms = add_menu_option(ms, NULL, run_redefine_menu);
    ms = add_menu_option(ms, NULL, men_options_windowed);
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
  static int blink_r = 120, blink_g = 200, blink_b = 40;

  if (ms) {

    scr_blit(spr_spritedata(menupicture), 0, 0);
    scr_blit(spr_spritedata(titledata), (SCREENWID - spr_spritedata(titledata)->w) / 2, 20);

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
        scr_putbar(hiscores_xpos - 5,
                   (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45 - 3,
                   clen, FONTHEI + 3, blink_r, blink_g, blink_b, 128);
      }
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos - scr_textlength(pos), (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, pos);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 10 + hiscores_maxlen_points - scr_textlength(points) , (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, points);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 20 + hiscores_maxlen_points, (t*(FONTHEI+1)) + spr_spritedata(titledata)->h + 45, name);
    }
    scr_color_ramp(&blink_r, &blink_g, &blink_b);

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
          gameresult = GAME_ABORTED;
      }
    } else {
      snd_wateroff();
    }
  } while (pts_lifesleft() && (tower != 8) && (gameresult != GAME_ABORTED));

  men_highscore(pts_points());
}

#ifdef HUNT_THE_FISH
static char *
men_main_bonusgame_proc(void *ms)
{
  if (ms) {
    snd_stoptitle();
    gam_newgame();
    lev_set_towercol(rand() % 256, rand() % 256, rand() % 256);
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
    switch (key_sdlkey2conv(tms->key, false)) {
    case fire_key:
      snd_stoptitle();
      main_game_loop();
      snd_playtitle();
      break;
    case right_key: currentmission = (currentmission + 1) % missioncount; break;
    case left_key: currentmission = (currentmission + missioncount - 1) % missioncount; break;
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
    (void)key_sdlkey();
    snd_playtitle();
  }
  return "Level Editor";
}

void men_main() {
  struct _menusystem *ms;

  ms = new_menu_system(NULL, men_options_background_proc, 0, spr_spritedata(titledata)->h + 30);

  if (!ms) return;

  snd_playtitle();

  ms = add_menu_option(ms, NULL, men_main_startgame_proc, SDLK_UNKNOWN, MOFLAG_PASSKEYS);
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, NULL, men_main_highscore_proc, SDLK_h);
  ms = add_menu_option(ms, NULL, men_options, SDLK_o);
  ms = add_menu_option(ms, NULL, men_main_leveleditor_proc, SDLK_e);
#ifdef HUNT_THE_FISH
  ms = add_menu_option(ms, NULL, men_main_bonusgame_proc);
#endif
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, "Quit", NULL, SDLK_q);

  ms->wraparound = true;

  ms = run_menu_system(ms);

  free_menu_system(ms);

  snd_stoptitle();
}

static int input_box_cursor_state = 0;

void
draw_input_box(int x, int y, int len, int cursor, char *txt)
{
  static int col_r = 0, col_g = 200, col_b = 120;

  scr_putbar(x, y, len * FONTMAXWID, FONTHEI, 0, 0, 0, 128);
  scr_writetext(x+1,y, txt);

  if ((input_box_cursor_state & 4) && ((cursor >= 0) && (cursor < len)))
    scr_putbar(x + scr_textlength(txt, cursor) + 1, y, FONTMINWID, FONTHEI,
               col_r, col_g, col_b, 128);
  scr_putrect(x,y, len * FONTMAXWID, FONTHEI, col_r, col_g, col_b, 128);
  input_box_cursor_state++;

  scr_color_ramp(&col_r, &col_g, &col_b);
}

void men_input_wait_proc(void) {
    if (menu_background_proc) (*menu_background_proc) ();
    scr_swap();
    dcl_wait();
}

void men_input(char *s, int max_len, int xpos, int ypos, const char *allowed) {
  char inp;
  int pos = strlen(s);
  int ztmp;
  bool ende = false;

  if (xpos < 0) xpos = (SCREENWID / 2) - max_len * (FONTWID / 2);
  if (xpos < 0) xpos = 0;

  if (ypos < 0) ypos = (SCREENHEI / 2) - (FONTHEI / 2);

  key_wait_for_none(men_input_wait_proc);

  do {

    if (key_keypressed(quit_action)) break;

    if (menu_background_proc) (*menu_background_proc) ();

    draw_input_box(xpos,ypos, max_len, pos, s);
    scr_swap();
    dcl_wait();

    inp = key_chartyped();

    switch (inp) {
    case 0: break;
    case 4: if ((unsigned)pos < strlen(s)) pos++; break;
    case 3: if (pos > 0) pos--; break;
    case 27: if (strlen(s)) {
      s[0] = '\0';
      pos = 0;
      break;
    } /* fall through */
    case '\r': ende = true; break;
    case 6:
      if (strlen(s) >= (unsigned)pos) {
        for (ztmp = pos; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
        s[ztmp] = '\0';
      }
      break;
    case '\b':
      if (pos > 0) {
        if (pos <= max_len) {
          for (ztmp = pos-1; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
          s[ztmp] = '\0';
        }
        pos--;
      }
      break;
    default:
      if ((((allowed != NULL) && (strchr(allowed, inp))) ||
           ((allowed == NULL) && (isalnum(inp) || (inp == ' ')))) &&
          (pos < max_len) && (strlen(s) >= (unsigned)pos) &&
          (strlen(s) < (unsigned)max_len)) {
        for (ztmp = max_len-1; ztmp >= pos; ztmp--) s[ztmp+1] = s[ztmp];
        s[pos] = inp;
        s[max_len] = '\0';
        pos++;
      }
      break;
    }
  } while (!ende);
}


static int congrats_placement = 0;

static void
congrats_background_proc(void)
{
  scr_blit(spr_spritedata(menupicture), 0, 0);
  scr_blit(spr_spritedata(titledata), (SCREENWID - spr_spritedata(titledata)->w) / 2, 20);

  scr_writetext_center(130, "Congratulations");
  if (congrats_placement == 0) {
    scr_writetext_center(170, "You've got the");
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

#if (SYSTEM == SYS_LINUX) 
    strncpy(scores[t].name, getenv("LOGNAME"), SCORENAMELEN);
    scores[t].name[SCORENAMELEN] = 0; //to be sure
#else
    scores[t].name[0] = 0;
#endif

#ifndef GAME_DEBUG_KEYS
    men_input(scores[t].name, SCORENAMELEN);

    scores[t].points = pt;

    savescores();
#endif /* GAME_DEBUG_KEYS */
  }

#ifdef USE_LOCKING
  unlink(SCOREFILE ".lck");
#endif

  show_scores(false, t);
}

void men_done(void) {
}

