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
#include "stars.h"
#include "robots.h"
#include "configuration.h"
#include "highscore.h"

#include <SDL_endian.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* really required? */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUMHISCORES 10
#define SCORENAMELEN 9
#define HISCORES_PER_PAGE 5

#define MENUTITLELEN  ((SCREENWID / FONTMINWID) + 1)
#define MENUOPTIONLEN MENUTITLELEN

/* menu option */
typedef struct {
   char oname[MENUOPTIONLEN];    /* text shown to user */
   menuopt_callback_proc oproc;  /* callback proc, supplies actions and the name */
   int  ostate;                  /* callback proc can use this */
   menuoptflags  oflags;         /* MOF_foo */
   SDLKey quickkey;              /* quick jump key; if user presses this key,
                                  * this menu option is hilited.
                                  */
} _menuoption;

typedef struct {
   char title[MENUTITLELEN];     /* title of the menu */
   int numoptions;               /* # of options in this menu */
   _menuoption *moption;         /* the options */
   menuopt_callback_proc mproc;
   menuopt_callback_proc timeproc;
   long curr_mtime;              /* current time this menu has been running */
   long mtime;                   /* time when to call timeproc */
   int hilited;                  /* # of the option that is hilited */
   int mstate;                   /* menu state, free to use by callback procs */
   int maxoptlen;                /* longest option name length, in pixels.
                                  * the hilite bar is slightly longer than this
                                  */
   bool exitmenu;                /* if true, the menu exits */
   bool wraparound;              /* if true, the hilite bar wraps around */
   int ystart;                   /* y pos where this menu begins, in pixels */
   int yhilitpos;                /* y pos of the hilite bar, in pixels */
   int opt_steal_control;        /* if >= 0, that option automagically gets
                                  * keys passed to it, and normal key/mouse
                                  * processing doesn't happen.
                                  */
   SDLKey key;                   /* the key that was last pressed */
} _menusystem;

static unsigned short menupicture, titledata;
static unsigned char currentmission = 0;

static menubg_callback_proc menu_background_proc = NULL;

/* create a new menu */
static _menusystem *new_menu_system(char *title, menuopt_callback_proc pr,
                                    int molen = 0, int ystart = 25);

/* add an option to the menu */
static _menusystem *add_menu_option(_menusystem *ms, char *name, menuopt_callback_proc pr,
                                    SDLKey quickkey = SDLK_UNKNOWN, menuoptflags flags = MOF_NONE, int state = 0);


void set_men_bgproc(menubg_callback_proc proc) {
   menu_background_proc = proc;
}

static
void men_reload_sprites(Uint8 what) {
  Uint8 pal[3*256];

  if (what & 1) {
    file fi(&dataarchive, menudat);

    scr_read_palette(&fi, pal);
    menupicture = scr_loadsprites(&restsprites, &fi, 1, 640, 480, false, pal, 0);
  }

  if (what & 2) {
    file fi(&dataarchive, titledat);

    scr_read_palette(&fi, pal);
    titledata = scr_loadsprites(&fontsprites, &fi, 1, SPR_TITLEWID, SPR_TITLEHEI, true, pal, config.use_alpha_font());
  }
}

void men_init(void) {
  men_reload_sprites(3);
}

static char *
men_yn_background_proc(void *ms)
{
  if (menu_background_proc) (*menu_background_proc) ();
  else {
      scr_blit(restsprites.data(menupicture), 0, 0);
      scr_blit(fontsprites.data(titledata), (SCREENWID - fontsprites.data(titledata)->w) / 2, 20);
  }
  return "";
}

static _menusystem *
new_menu_system(char *title, menuopt_callback_proc pr, int molen, int ystart)
{
  _menusystem *ms = new _menusystem;

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
    ms->curr_mtime = ms->hilited = 0;
    ms->ystart = ystart;
    ms->key = SDLK_UNKNOWN;
    ms->mtime = ms->yhilitpos = ms->opt_steal_control = -1;
    ms->timeproc = NULL;
  }

  return ms;
}

static _menusystem *
add_menu_option(_menusystem *ms,
                char *name,
                menuopt_callback_proc pr,
                SDLKey quickkey,
                menuoptflags flags,
                int state) {
  _menuoption *tmp;
  int olen = 0;

  if (!ms) return ms;

  tmp = new _menuoption[ms->numoptions+1];

  if (!tmp) return ms;

  memcpy(tmp, ms->moption, sizeof(_menuoption)*ms->numoptions);
  delete [] ms->moption;

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

_menusystem *
set_menu_system_timeproc(_menusystem *ms, long t, menuopt_callback_proc pr)
{
    if (!ms) return ms;
    
    ms->timeproc = pr;
    ms->mtime = t;
    
    return ms;
}

static void
free_menu_system(_menusystem *ms)
{
  if (!ms) return;

  delete [] ms->moption;
  ms->numoptions = 0;
  ms->mstate = 0;
  ms->mproc = NULL;
  delete ms;
}

void
draw_menu_system(_menusystem *ms, Uint16 dx, Uint16 dy)
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

  if (ms->mproc) {
    (*ms->mproc) (ms);
    menu_background_proc = NULL;
  } else men_yn_background_proc(ms);

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
      if (dx >= minx && dx <= maxx && dy >= miny && dy <= maxy) {
        newhilite = y + offs;
        ms->curr_mtime = 0;
      }
      if (y + offs == ms->hilited) {
        if (ms->yhilitpos == -1) {
          ms->yhilitpos = miny;
        } else {
          if (ms->yhilitpos < miny) {
            ms->yhilitpos += ((miny - ms->yhilitpos + 3) / 4)+1;
            if (ms->yhilitpos > miny) ms->yhilitpos = miny;
          } else if (ms->yhilitpos > miny) {
            ms->yhilitpos -= ((ms->yhilitpos - miny + 3) / 4)+1;
            if (ms->yhilitpos < miny) ms->yhilitpos = miny;
          }
        }
        scr_putbar((SCREENWID - ms->maxoptlen - 8) / 2, ms->yhilitpos - 3,
                   ms->maxoptlen + 8, FONTHEI + 3,
                   color_r, color_g, color_b, (config.use_alpha_darkening())?128:255);
      }
    }
  }

  maxy = y;

  for (y = 0; y < maxy; y++) {
    if (strlen(ms->moption[y+offs].oname)) {
      miny = ms->ystart + (y + titlehei)*FONTHEI;
      if ((ms->moption[y+offs].oflags & MOF_LEFT))
	  scr_writetext((SCREENWID - ms->maxoptlen) / 2 + 4, miny,
			 ms->moption[y+offs].oname);
      else
      if ((ms->moption[y+offs].oflags & MOF_RIGHT))
	  scr_writetext((SCREENWID + ms->maxoptlen) / 2 - 4 
			- scr_textlength(ms->moption[y+offs].oname), miny,
			 ms->moption[y+offs].oname);
      else
	  scr_writetext_center(miny, ms->moption[y+offs].oname);
    }
  }

  if (newhilite >= 0) ms->hilited = newhilite;

  scr_color_ramp(&color_r, &color_g, &color_b);

  scr_swap();
  dcl_wait();
}


void
menu_system_caller(_menusystem *ms)
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


static _menusystem *
run_menu_system(_menusystem *ms)
{
  Uint16 x,y;
  ttkey bttn;
  bool stolen = false;

  if (!ms) return ms;

  /* find the first option with text */
  if (!strlen(ms->moption[ms->hilited].oname))
      do {
	  ms->hilited = (ms->hilited + 1) % ms->numoptions;
      } while (!strlen(ms->moption[ms->hilited].oname));

  (void)key_sdlkey();

  do {

    stolen = false;
    bttn = no_key;
    x = y = 0;

    ms->key = SDLK_UNKNOWN;

    if ((ms->curr_mtime++ >= ms->mtime) && ms->timeproc) {
	(void) (*ms->timeproc) (ms);
	ms->curr_mtime = 0;
    }
      
    if ((ms->opt_steal_control >= 0) &&
        (ms->opt_steal_control < ms->numoptions) &&
        ms->moption[ms->opt_steal_control].oproc) {
      ms->key = key_sdlkey();
      ms->hilited = ms->opt_steal_control;
      stolen = true;
    } else {
	if (!config.fullscreen() && !key_mouse(&x, &y, &bttn) && bttn)
	  ms->key = key_conv2sdlkey(bttn, false);
	else ms->key = key_sdlkey();
    }

    draw_menu_system(ms, x, y);

    if ((ms->key != SDLK_UNKNOWN) || stolen) {

      if (!stolen) {
	ms->curr_mtime = 0;
        for (int tmpz = 0; tmpz < ms->numoptions; tmpz++)
          if (ms->moption[tmpz].quickkey == ms->key) {
            ms->hilited = tmpz;
            ms->key = SDLK_UNKNOWN;
            break;
          }
      }

      if ((((ms->moption[ms->hilited].oflags & MOF_PASSKEYS)) || stolen) &&
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
	    int tmpz = ms->hilited;
            if (ms->hilited > 0) {
	      do {
		  if (ms->hilited < 0) {
		      ms->hilited = tmpz;
		      break;
		  }
		  ms->hilited--;
              } while (!strlen(ms->moption[ms->hilited].oname));
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
  _menusystem *ms = new_menu_system("DEBUG MENU", NULL, 0, SCREENHEI / 5);

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
  _menusystem *ms = (_menusystem *)tmp;
  if (ms) {
    ms->mstate = 1;
    ms->exitmenu = true;
    return NULL;
  } else return "Yes";
}

static char *
men_yn_option_no(void *tmp)
{
  _menusystem *ms = (_menusystem *)tmp;
  if (ms) {
    ms->mstate = 0;
    ms->exitmenu = true;
    return NULL;
  } else return "No";
}

unsigned char men_yn(char *s, bool defchoice) {
  _menusystem *ms = new_menu_system(s, NULL, 0, SCREENHEI / 5);

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
men_main_background_proc(void *ms)
{
  if (ms) {
    scr_blit(restsprites.data(menupicture), 0, 0);
    scr_blit(fontsprites.data(titledata), (SCREENWID - fontsprites.data(titledata)->w) / 2, 20);
    return NULL;
  }
  return "";
}

#define REDEFINEREC 5
static int times_called = 0;
static char *redefine_menu_up(void *tms) {
  _menusystem *ms = (_menusystem *)tms;
  static char buf[50];
  const char *code[REDEFINEREC] = {"Up", "Down", "Left", "Right", "Fire"};
  char *keystr;
  static int blink, times_called;
  const ttkey key[REDEFINEREC] = {up_key, down_key, left_key, right_key, fire_key};
  const char *redef_fmt = "%s:  %s";
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
    else keystr = "";
    snprintf(buf, 50, redef_fmt, code[ms->hilited % REDEFINEREC], keystr);
  } else {
    keystr = SDL_GetKeyName(key_conv2sdlkey(key[times_called], true));
    snprintf(buf, 50, redef_fmt, code[times_called], keystr);
    times_called = (times_called + 1) % REDEFINEREC;
  }
  return buf;
}

static char *game_options_menu_password(void *prevmenu) {
  static char buf[50];
  char pwd[PASSWORD_LEN+1];

  if (prevmenu) {
    /* one more character to also copy the termination */
    strncpy(pwd, config.curr_password(), PASSWORD_LEN+1);
    while (!men_input(pwd, PASSWORD_LEN, -1, -1, PASSWORD_CHARS)) ;
    config.curr_password(pwd);
	/* FIXME: change -1, -1 to correct position; Need to fix menu system
	   first... */
    }
    snprintf(buf, 50, "Password: %s", config.curr_password());
    return buf;
}

static char *game_options_menu_statustop(void *prevmenu) {
    if (prevmenu) {
	config.status_top(!config.status_top());
    }
    if (config.status_top()) return "Status on top \x04";
    else return "Status on top \x03";
}

static char *game_options_menu_lives(void *prevmenu) {
    static char buf[50];
    int i;
    if (prevmenu) {
	_menusystem *tms = (_menusystem *)prevmenu;
	switch (key_sdlkey2conv(tms->key, false)) {
	    case right_key: 
	        config.start_lives(config.start_lives() + 1);
	        if (config.start_lives() > 3) config.start_lives(3);
	        break;
	    case left_key:  
	        config.start_lives(config.start_lives() - 1);
	        if (config.start_lives() < 1) config.start_lives(1);
	        break;
	    default: return NULL;
	}
    }
    sprintf(buf, "Lives: ");
    for (i = 0; i < config.start_lives(); i++)
      sprintf(buf + strlen(buf), "%c", fonttoppler);
    return buf;
}

static char *
game_options_menu_speed(void *prevmenu)
{
    // Changing game_speed during a game has no effect until a
    // a new game is started.
    static char buf[50];
    if (prevmenu) {
	_menusystem *tms = (_menusystem *)prevmenu;
	switch (key_sdlkey2conv(tms->key, false)) {
	    case right_key: 
	        config.game_speed(config.game_speed() + 1);
	        if (config.game_speed() > MAX_GAME_SPEED) config.game_speed(MAX_GAME_SPEED);
	        break;
	    case left_key:  
	        config.game_speed(config.game_speed() - 1);
	        if (config.game_speed() < 0) config.game_speed(0);
	        break;
	    case fire_key:
	        config.game_speed((config.game_speed() + 1) % (MAX_GAME_SPEED+1));
	        break;
	    default: return NULL;
	}
    }
    snprintf(buf, 50, "Game Speed: %i", config.game_speed());
    return buf;
}

static char *men_game_options_menu(void *prevmenu) {
    static char s[20] = "Game Options";
    if (prevmenu) {
	_menusystem *ms = new_menu_system(s, NULL, 0, fontsprites.data(titledata)->h+30);

	ms = add_menu_option(ms, NULL, game_options_menu_password, SDLK_UNKNOWN, MOF_LEFT);
	ms = add_menu_option(ms, NULL, game_options_menu_lives, SDLK_UNKNOWN, 
			     (menuoptflags)((int)MOF_PASSKEYS|(int)MOF_LEFT));
	ms = add_menu_option(ms, NULL, game_options_menu_statustop);
	ms = add_menu_option(ms, NULL, game_options_menu_speed, SDLK_UNKNOWN,
			     (menuoptflags)((int)MOF_PASSKEYS|(int)MOF_LEFT));
	ms = add_menu_option(ms, NULL, NULL);
	ms = add_menu_option(ms, "Back", NULL);
	
	ms = run_menu_system(ms);
	
	free_menu_system(ms);
    }
    return s;
}

static char *run_redefine_menu(void *prevmenu) {
  if (prevmenu) {
    _menusystem *ms = new_menu_system("Redefine Keys", NULL, 0, fontsprites.data(titledata)->h+30);

    times_called = 0;

    ms = add_menu_option(ms, NULL, redefine_menu_up, SDLK_UNKNOWN, MOF_LEFT);
    ms = add_menu_option(ms, NULL, redefine_menu_up, SDLK_UNKNOWN, MOF_LEFT);
    ms = add_menu_option(ms, NULL, redefine_menu_up, SDLK_UNKNOWN, MOF_LEFT);
    ms = add_menu_option(ms, NULL, redefine_menu_up, SDLK_UNKNOWN, MOF_LEFT);
    ms = add_menu_option(ms, NULL, redefine_menu_up, SDLK_UNKNOWN, MOF_LEFT);
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
    config.fullscreen(!config.fullscreen());
    scr_reinit();
    SDL_ShowCursor(config.fullscreen() ? 0 : 1);
  }
  if (config.fullscreen()) return "Fullscreen \x04";
  else return "Fullscreen \x03";
}

static char *
men_options_sounds(void *ms)
{
  if (ms) {
    if (config.nosound()) {
      config.nosound(false);
      snd_init();
    } else {
      snd_stoptitle();
      snd_stoptgame();
      snd_done();
      config.nosound(true);
    }
  }
  if (config.nosound()) return "Sounds \x03";
  else return "Sounds \x04";
}

static void
reload_font_graphics(void) {
  fontsprites.freedata();

  scr_reload_sprites(RL_FONT);
  men_reload_sprites(2);
}

static void
reload_robot_graphics(void) {
  objectsprites.freedata();
  scr_reload_sprites(RL_OBJECTS);
}

static void
reload_layer_graphics(void) {
  layersprites.freedata();
  scr_reload_sprites(RL_SCROLLER);
}

static char *
men_alpha_font(void *ms)
{
  if (ms) {
    config.use_alpha_font(!config.use_alpha_font());
    reload_font_graphics();
  }
  if (config.use_alpha_font()) return "Font alpha \x04";
  else return "Font alpha \x03";
}

static char *
men_alpha_sprites(void *ms)
{
  if (ms) {
    config.use_alpha_sprites(!config.use_alpha_sprites());
    reload_robot_graphics();
  }
  if (config.use_alpha_sprites()) return "Sprites alpha \x04";
  else return "Sprites alpha \x03";
}

static char *
men_alpha_layer(void *ms)
{
  if (ms) {
    config.use_alpha_layers(!config.use_alpha_layers());
    reload_layer_graphics();
  }
  if (config.use_alpha_layers()) return "Scroller alpha \x04";
  else return "Scroller alpha \x03";
}

static char *
men_alpha_menu(void *ms)
{
  if (ms) {
    config.use_alpha_darkening(!config.use_alpha_darkening());
  }
  if (config.use_alpha_darkening()) return "Shadowing \x04";
  else return "Shadowing \x03";
}

static char *
men_waves_menu(void *ms)
{
  if (ms) {
    _menusystem *tms = (_menusystem *)ms;
    switch (key_sdlkey2conv(tms->key, false)) {
    case fire_key:
      config.waves_type((config.waves_type() + 1) % configuration::num_waves);
      break;
    case right_key:
      config.waves_type(config.waves_type() + 1);
      if (config.waves_type() >= configuration::num_waves) config.waves_type(configuration::num_waves - 1);
      break;
    case left_key:
      config.waves_type(config.waves_type() - 1);
      if (config.waves_type() < 0) config.waves_type(0);
      break;
    default: return NULL;
    }
  }
  switch(config.waves_type()) {
  case configuration::waves_nonreflecting: return "Nonreflecting waves";
  case configuration::waves_simple: return "Simple waves";
  case configuration::waves_expensive: return "Expensive waves";
  default: return "Error";
  }
}

static char *
men_full_scroller(void *ms)
{
  if (ms) {
    config.use_full_scroller(!config.use_full_scroller());
  }
  if (config.use_full_scroller()) return "Complete Scroller";
  else return "2 layers Scoller";
}


static char *
men_alpha_options(void *mainmenu) {
  static char s[20] = "Alpha Options";
  if (mainmenu) {

    _menusystem *ms = new_menu_system(s, NULL, 0, fontsprites.data(titledata)->h+30);

    if (!ms) return NULL;

    ms = add_menu_option(ms, NULL, men_alpha_font, SDLK_UNKNOWN, MOF_RIGHT);
    ms = add_menu_option(ms, NULL, men_alpha_sprites, SDLK_UNKNOWN, MOF_RIGHT);
    ms = add_menu_option(ms, NULL, men_alpha_layer, SDLK_UNKNOWN, MOF_RIGHT);
    ms = add_menu_option(ms, NULL, men_alpha_menu, SDLK_UNKNOWN, MOF_RIGHT);

    ms = add_menu_option(ms, NULL, NULL);
    ms = add_menu_option(ms, "Back", NULL);

    ms = run_menu_system(ms);

    free_menu_system(ms);
  }
  return s;
}

static char *
men_options_graphic(void *mainmenu) {
  static char s[20] = "Graphics";
  if (mainmenu) {

    _menusystem *ms = new_menu_system(s, NULL, 0, fontsprites.data(titledata)->h+30);

    if (!ms) return NULL;

    ms = add_menu_option(ms, NULL, men_options_windowed);
    ms = add_menu_option(ms, NULL, men_alpha_options);
    ms = add_menu_option(ms, NULL, men_waves_menu, SDLK_UNKNOWN, MOF_PASSKEYS);
    ms = add_menu_option(ms, NULL, men_full_scroller, SDLK_UNKNOWN, MOF_RIGHT);

    ms = add_menu_option(ms, NULL, NULL);
    ms = add_menu_option(ms, "Back", NULL);

    ms = run_menu_system(ms);

    free_menu_system(ms);
  }
  return s;
}

static char *
men_options(void *mainmenu) {
  static char s[20] = "Options";
  if (mainmenu) {

    _menusystem *ms = new_menu_system(s, NULL, 0, fontsprites.data(titledata)->h+30);

    if (!ms) return NULL;

    ms = add_menu_option(ms, NULL, men_game_options_menu);
    ms = add_menu_option(ms, NULL, run_redefine_menu);
    ms = add_menu_option(ms, NULL, men_options_graphic);
    ms = add_menu_option(ms, NULL, men_options_sounds);

    ms = add_menu_option(ms, NULL, NULL);
    ms = add_menu_option(ms, "Back", NULL);

    ms = run_menu_system(ms);

    free_menu_system(ms);
  }
  return s;
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
  Uint32 pt;
  Uint8 tw;

  static char buf1[SCORENAMELEN + 5];
  static char buf2[SCORENAMELEN + 5];
  static char buf3[SCORENAMELEN + 5];

  buf1[0] = buf2[0] = buf3[0] = '\0';

  hsc_entry(p, buf3, &pt, &tw);

  snprintf(buf1, SCORENAMELEN + 5, "%i.", p + 1);
  snprintf(buf2, SCORENAMELEN + 5, "%i", pt);
    
  *pos = buf1;
  *points = buf2;
  *name = buf3;
}

void
calc_hiscores_maxlen(int *max_pos, int * max_points, int *max_name)
{
  for (int x = 0; x < hsc_entries(); x++) {
    char *a, *b, *c;
    int clen;

    get_hiscores_string(x, &a, &b, &c);

    clen = scr_textlength(a);
    if (clen > *max_pos) *max_pos = clen;

    clen = scr_textlength(b);
    if (clen < 64) clen = 64; 
    if (clen > *max_points) *max_points = clen;

    clen = scr_textlength(c);
    if (clen > *max_name) *max_name = clen;
  }
}

static char *
men_hiscores_background_proc(void *ms)
{
  static int blink_r = 120, blink_g = 200, blink_b = 40;
  static int next_page = 0;

  if (ms) {

    scr_blit(restsprites.data(menupicture), 0, 0);
    scr_blit(fontsprites.data(titledata), (SCREENWID - fontsprites.data(titledata)->w) / 2, 20);

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
      } else {
        bool filled_page = false;
        bool firstpage = (hiscores_pager == 0);
        int pager = (hiscores_pager + 1) % (NUMHISCORES / HISCORES_PER_PAGE);
        for (int tmp = 0; tmp < HISCORES_PER_PAGE; tmp++) {
//          int cs = tmp + (pager * HISCORES_PER_PAGE);
//          if (scores[cs].points || strlen(scores[cs].name)) {
            filled_page = true;
            break;
//          }
        }
        if (!filled_page && firstpage) {
          hiscores_timer = 0;
          break;
        } else {
          hiscores_state = 2;
          next_page = pager;
        }
      }
    case 2: /* move the scores out */
      if (hiscores_xpos > -(hiscores_maxlen + 40)) {
        hiscores_timer = 0;
        hiscores_xpos -= 10;
        break;
      } else {
        hiscores_state = 0;
        hiscores_xpos = SCREENWID;
        hiscores_pager = next_page;
      }
    default: break;
    }
    for (int t = 0; t < HISCORES_PER_PAGE; t++) {
      int cs = t + (hiscores_pager * HISCORES_PER_PAGE);
      int ypos = (t*(FONTHEI+1)) + fontsprites.data(titledata)->h + FONTHEI*2;
      char *pos, *points, *name;
      get_hiscores_string(cs, &pos, &points, &name);
      if (cs == hiscores_hilited) {
        int clen = hiscores_maxlen_pos + hiscores_maxlen_points + hiscores_maxlen_name + 20 * 2 + 20;
        scr_putbar(hiscores_xpos - 5, ypos - 3,
                   clen, FONTHEI + 3, blink_r, blink_g, blink_b, (config.use_alpha_darkening())?128:255);
      }
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos - scr_textlength(pos), ypos, pos);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 20 + hiscores_maxlen_points - scr_textlength(points), ypos, points);
      scr_writetext(hiscores_xpos + hiscores_maxlen_pos + 20 + 20 + hiscores_maxlen_points, ypos, name);
    }
    scr_color_ramp(&blink_r, &blink_g, &blink_b);
  }
  return "HighScores";
}

static void show_scores(bool back = true, int mark = -1) {
  static char buf[50];
  snprintf(buf, 50, "Scores for %s", lev_missionname(currentmission));
  _menusystem *ms = new_menu_system(buf, men_hiscores_background_proc, 0, fontsprites.data(titledata)->h + 30);

  if (!ms) return;

  hsc_select(lev_missionname(currentmission));

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

  /* fake options; the empty lines are used by the background proc */
  for (int tmpz = 0; tmpz < HISCORES_PER_PAGE; tmpz++) ms = add_menu_option(ms, NULL, NULL);

  if (back)
    ms = add_menu_option(ms, "Back", NULL);
  else
    ms = add_menu_option(ms, "OK", NULL);

  ms = run_menu_system(ms);

  free_menu_system(ms);
}

void
main_game_loop()
{
  unsigned char tower = 0;
  Uint8 anglepos;
  Uint16 resttime;
  int demo = 0;
  int gameresult;
  Uint16 *tmpbuf = NULL;

  lev_loadmission(currentmission);
    
  tower = lev_tower_passwd_entry(config.curr_password());

  gam_newgame();
  bns_restart();

  do {
    snd_wateron();
    do {
      gam_loadtower(tower);
      scr_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());
      snd_watervolume(128);
      snd_playtgame();
      gam_arrival();
      gameresult = gam_towergame(anglepos, resttime, demo, &tmpbuf);
      snd_stoptgame();
    } while ((gameresult == GAME_DIED) && pts_lifesleft());

    if (gameresult == GAME_FINISHED) {
      gam_pick_up(anglepos, resttime);

      snd_wateroff();
      tower++;

      if (tower < lev_towercount()) {

        // load next tower, because its colors will be needed for bonus game
        gam_loadtower(tower);

        if (!bns_game())
          gameresult = GAME_ABORTED;
      }
    } else {
      snd_wateroff();
    }
  } while (pts_lifesleft() && (tower < lev_towercount()) && (gameresult != GAME_ABORTED));

  if (gameresult != GAME_ABORTED)
      men_highscore(pts_points(), (tower >= lev_towercount()) ? tower : -1);
}

#ifdef HUNT_THE_FISH
static char *
men_main_bonusgame_proc(void *ms)
{
  if (ms) {
    snd_stoptitle();
    gam_newgame();
    scr_settowercolor(rand() % 256, rand() % 256, rand() % 256);
    lev_set_towercol(rand() % 256, rand() % 256, rand() % 256);
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
    _menusystem *tms = (_menusystem *)ms;
    int missioncount = lev_missionnumber();
    switch (key_sdlkey2conv(tms->key, false)) {
    case fire_key:
      dcl_update_speed(config.game_speed());
      snd_stoptitle();
      main_game_loop();
      snd_playtitle();
      dcl_update_speed(MENU_DCLSPEED);
      break;
    case right_key: currentmission = (currentmission + 1) % missioncount; break;
    case left_key: currentmission = (currentmission + missioncount - 1) % missioncount; break;
    default: return NULL;
    }
  }
  static char s[30];
  snprintf(s, 30, "%c Start: %s %c", fontptrleft, lev_missionname(currentmission), fontptrright);
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

static char *
men_main_timer_proc(void *ms)
{
    if (ms) {
	Uint8 num_demos = 0;
	Uint8 demos[256];
	Uint16 miss = rand() % lev_missionnumber();
	Uint8 num_towers;

	int demolen;
	Uint16 *demobuf;
	Uint8 anglepos;
	Uint16 resttime;

	for (int tmpm = 0; (tmpm < lev_missionnumber()) && (num_demos == 0); tmpm++) {
	    Uint16 tmiss = (miss + tmpm) % lev_missionnumber();
	    lev_loadmission(tmiss);

	    num_towers = lev_towercount();

	    for (Uint8 idx = 0; (idx < num_towers) && (num_demos < 256); idx++) {
		lev_selecttower(idx);
		lev_get_towerdemo(demolen, demobuf);
		if (demolen) demos[num_demos++] = idx;
	    }
	}

	if (num_demos < 1) return NULL;

	lev_selecttower(demos[rand() % num_demos]);
	lev_get_towerdemo(demolen, demobuf);

	dcl_update_speed(config.game_speed());
	snd_stoptitle();
	gam_newgame();
	snd_wateron();
	scr_settowercolor(lev_towercol_red(), lev_towercol_green(), lev_towercol_blue());
	snd_watervolume(128);
	snd_playtgame();
	rob_initialize();
	(void)gam_towergame(anglepos, resttime, demolen, &demobuf);
	snd_stoptgame();
	snd_wateroff();
	dcl_update_speed(MENU_DCLSPEED);

	snd_playtitle();
    }
    return NULL;
}

void men_main() {
  _menusystem *ms;

  ms = new_menu_system(NULL, men_main_background_proc, 0, fontsprites.data(titledata)->h + 30);

  if (!ms) return;

  snd_playtitle();

  ms = set_menu_system_timeproc(ms, 700, men_main_timer_proc);
    
  ms = add_menu_option(ms, NULL, men_main_startgame_proc, SDLK_s, MOF_PASSKEYS);
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

static char *
men_game_return2game(void *ms)
{
  if (ms) {
      _menusystem *tms = (_menusystem *)ms;
      tms->exitmenu = true;
      tms->mstate = 0;
  }
  return "Return to Game";
}

static char *
men_game_leavegame(void *ms)
{
  if (ms) {
      _menusystem *tms = (_menusystem *)ms;
      tms->exitmenu = true;
      tms->mstate = 1;
  }
  return "Quit Game";
}

bool men_game() {
  _menusystem *ms;
  bool do_quit;
  int  speed = dcl_update_speed(MENU_DCLSPEED);
    
  ms = new_menu_system(NULL, NULL, 0, fontsprites.data(titledata)->h + 30);

  if (!ms) return 0;

  ms = add_menu_option(ms, NULL, men_game_return2game);
  ms = add_menu_option(ms, NULL, men_options, SDLK_o);
  ms = add_menu_option(ms, NULL, NULL);
  ms = add_menu_option(ms, NULL, men_game_leavegame);
    
  ms->wraparound = true;

  ms = run_menu_system(ms);

  do_quit = ms->mstate != 0;

  free_menu_system(ms);

  dcl_update_speed(speed);

  return do_quit;
}

static int input_box_cursor_state = 0;

void
draw_input_box(int x, int y, int len, int cursor, char *txt)
{
  static int col_r = 0, col_g = 200, col_b = 120;
  int nlen = len, slen = len;
  int arrows = 0;

  if ((len+3)*FONTMAXWID > SCREENWID)
      nlen = (SCREENWID / FONTMAXWID) - 3;

  if (x < 0) x = (SCREENWID / 2) - nlen * (FONTMAXWID / 2);
  if (x < 0) x = 0;
  if (y < 0) y = (SCREENHEI / 2) - (FONTHEI / 2);

  scr_putbar(x, y, nlen * FONTMAXWID, FONTHEI, 0, 0, 0, (config.use_alpha_darkening())?128:255);

  if (scr_textlength(txt) >= nlen*FONTMAXWID) {
      while ((cursor >= 0) &&
	     (scr_textlength(txt, cursor+(nlen/2)) >= (nlen)*FONTMAXWID)) {
	  cursor--;
	  txt++;
	  arrows = 1;
      }
  }
  if (scr_textlength(txt) >= nlen*FONTMAXWID) {
      arrows |= 2;
      while ((slen > 0) && (scr_textlength(txt, slen) >= nlen*FONTMAXWID)) slen--;
  }

  scr_writetext(x+1,y, txt, slen);

  if ((input_box_cursor_state & 4) && (cursor >= 0))
    scr_putbar(x + scr_textlength(txt, cursor) + 1, y, FONTMINWID, FONTHEI,
               col_r, col_g, col_b, (config.use_alpha_darkening())?128:255);
  scr_putrect(x,y, nlen * FONTMAXWID, FONTHEI, col_r, col_g, col_b, 255);

  if ((arrows & 1)) scr_writetext(x-FONTMAXWID,y, "\x08"); //fontptrright
  if ((arrows & 2)) scr_writetext(x+(nlen*FONTMAXWID),y, "\x06"); //fontptrleft
    
  input_box_cursor_state++;

  scr_color_ramp(&col_r, &col_g, &col_b);
}

void men_input_wait_proc(void) {
    if (menu_background_proc) (*menu_background_proc) ();
    scr_swap();
    dcl_wait();
}

bool men_input(char *origs, int max_len, int xpos, int ypos, const char *allowed) {
  SDLKey sdlinp;
  char inpc;
  ttkey inptt;
  static int pos = strlen(origs);
  int ztmp;
  static char s[256];
  static bool copy_origs = true;
  bool restore_origs = false;
  bool ende = false;

  if ((strlen(origs) >= 256)) return true;

  if (copy_origs) {
    strcpy(s, origs);
    copy_origs = false;
    pos = strlen(origs);
  }

  (void)key_readkey();

  if (menu_background_proc) (*menu_background_proc) ();

  draw_input_box(xpos,ypos, max_len, pos, s);
  scr_swap();
  dcl_wait();

  key_keydatas(sdlinp, inptt, inpc);

  switch (sdlinp) {
  case SDLK_RIGHT: if ((unsigned)pos < strlen(s)) pos++; break;
  case SDLK_LEFT: if (pos > 0) pos--; break;
  case SDLK_ESCAPE:if (strlen(s)) {
    s[0] = '\0';
    pos = 0;
    restore_origs = false;
  } else {
    restore_origs = true;
    ende = true;
  }
  break;
  case SDLK_RETURN: restore_origs = false; copy_origs = true; ende = true;
  break;
  case SDLK_DELETE:
    if (strlen(s) >= (unsigned)pos) {
      for (ztmp = pos; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
      s[ztmp] = '\0';
    }
    break;
  case SDLK_BACKSPACE:
    if (pos > 0) {
      if (pos <= max_len) {
        for (ztmp = pos-1; ztmp < max_len-1; ztmp++) s[ztmp] = s[ztmp+1];
        s[ztmp] = '\0';
      }
      pos--;
    }
    break;
  default:
    if (pos >= max_len || (inpc < ' ')) break;
    if (allowed) {
      if (!strchr(allowed, inpc)) {
        if (strchr(allowed, toupper(inpc))) inpc = toupper(inpc);
        else
          if (strchr(allowed, tolower(inpc))) inpc = tolower(inpc);
          else break;
      }
    } else {
      if (inpc < ' ' || inpc > 'z') break;
    }
    if ((strlen(s) >= (unsigned)pos) &&
        (strlen(s) < (unsigned)max_len)) {
      for (ztmp = max_len-1; ztmp >= pos; ztmp--) s[ztmp+1] = s[ztmp];
      s[pos] = inpc;
      s[max_len] = '\0';
      pos++;
    }
    break;
  }
  if (ende) {
    if (!restore_origs) strcpy(origs, s);
    s[0] = 0;
    copy_origs = true;
  } else {
    copy_origs = false;
  }
  return ende;
}

static void
congrats_background_proc(void)
{
  scr_blit(restsprites.data(menupicture), 0, 0);
  scr_blit(fontsprites.data(titledata), (SCREENWID - fontsprites.data(titledata)->w) / 2, 20);

  scr_writetext_center(130, "Congratulations! You are");
  scr_writetext_center(170, "probably good enough to");
  scr_writetext_center(210, "enter the highscore table!");

  scr_writetext_center(270, "Please enter your name");
}
  
/* highscores, after the game
 * pt = points, 
 * twr = tower reached, -1 = mission finished
 */
static void men_highscore(unsigned long pt, int twr) {

  Uint8 pos = 0xff;

#ifndef GAME_DEBUG_KEYS

  hsc_select(lev_missionname(currentmission));

  /* check, if there is a chance at all to get into the list,
   * if not we don't need to lock the highscoretable
   */
  if (hsc_canEnter(pt)) {

    set_men_bgproc(congrats_background_proc);

    char name[SCORENAMELEN+1];

#if (SYSTEM == SYS_LINUX)
    /* copy the login name into the name entered into the highscore table */
    strncpy(name, getenv("LOGNAME"), SCORENAMELEN);
    name[SCORENAMELEN] = 0; // to be sure we have a terminated string
#else
    /* on systems without login we have no name */
    name[0] = 0;
#endif

    while (!men_input(name, SCORENAMELEN)) ;

    pos = hsc_enter(pt, twr, name);
  }

#endif /* GAME_DEBUG_KEYS */

  show_scores(false, pos);
}

void men_done(void) {
}

