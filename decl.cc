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

#include "decl.h"
#include "keyb.h"

#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#if (SYSTEM != SYS_WINDOWS)

#include <pwd.h>
#include <dirent.h>

#endif

bool fullscreen;
bool nosound;
bool use_water = true;
bool use_alpha_sprites = false;
bool use_alpha_layers = false;
bool use_alpha_font = false;
bool use_alpha_darkening = false;
bool use_waves = false;
bool status_top = true;  /* is status line top or bottom of screen? */
int  editor_towerpagesize = -1;
int  editor_towerstarthei = -5;
char curr_password[PASSWORD_LEN+1] = "";
int  start_lives = 3;
bool use_unicode_input = true;
char editor_towername[TOWERNAMELEN+1] = "";

typedef enum {
    CT_BOOL,
    CT_STRING,
    CT_INT,
    CT_KEY
} cnf_type;

struct _config_data {
    char     *cnf_name;
    cnf_type  cnf_typ;
    void     *cnf_var;
    long      maxlen;
};

#define CNF_BOOL(a,b) {a, CT_BOOL, b, 0}
#define CNF_CHAR(a,b,c) {a, CT_STRING, b, c}
#define CNF_INT(a,b) {a, CT_INT, b, 0}
#define CNF_KEY(a,b) {a, CT_KEY, NULL, b}

static const struct _config_data config_data[] = {
    CNF_BOOL( "fullscreen",          &fullscreen ),
    CNF_BOOL( "nosound",             &nosound ),
    CNF_CHAR( "editor_towername",    &editor_towername, TOWERNAMELEN ),
    CNF_BOOL( "use_alpha_sprites",   &use_alpha_sprites ),
    CNF_BOOL( "use_alpha_font",      &use_alpha_font ),
    CNF_BOOL( "use_alpha_layers",    &use_alpha_layers ),
    CNF_BOOL( "use_alpha_darkening", &use_alpha_darkening ),
    CNF_BOOL( "use_waves",           &use_waves ),
    CNF_BOOL( "status_top",          &status_top ),
    CNF_INT(  "editor_pagesize",     &editor_towerpagesize ),
    CNF_INT(  "editor_towerstarthei",&editor_towerstarthei ),
    CNF_KEY(  "key_fire",             fire_key ),
    CNF_KEY(  "key_up",               up_key ),
    CNF_KEY(  "key_down",             down_key ),
    CNF_KEY(  "key_left",             left_key ),
    CNF_KEY(  "key_right",            right_key ),
    CNF_CHAR( "password",            &curr_password, PASSWORD_LEN ),
    CNF_BOOL( "use_unicode_input",   &use_unicode_input),
};

bool str2bool(char *s) {
    if (s) {
	if (!strcmp("yes", s) || !strcmp("true", s)) return true;
	else return (atoi(s) != 0);
    }
    return false;
}

void dcl_wait(void) {
  if (!key_keypressed(quit_action)) {
      static Uint32 last;
      while ((SDL_GetTicks() - last) < 55*1 ) SDL_Delay(2);
      last = SDL_GetTicks();
  }
}

/* returns true, if file exists */
static bool dcl_fileexists(char *n) {
  FILE *f = fopen(n, "r");

  if (f) {
    fclose(f);
    return true;
  } else
    return false;
}

char * homedir()
{

#if (SYSTEM == SYS_LINUX)

  struct passwd *pw = getpwuid(getuid());
  static char c = 0;
  if (!pw) {

    // empty string
    return &c;
  }
  return pw->pw_dir;

#else

  return "./";

#endif

}

/* checks if home/.toppler exists and creates it, if not */
static void checkdir(void) {

#if (SYSTEM == SYS_LINUX)

  char n[200];

  sprintf(n, "%s/.toppler", homedir());

  DIR *d = opendir(n);

  if (!d) {
    mkdir(n, S_IRWXU);
  }

#endif

}

FILE *open_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  // look into actual directory
  if (dcl_fileexists(name))
    return fopen(name, "rb");

  // look into the data dir
  char n[200];

  sprintf(n, TOP_DATADIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, "rb");

  return NULL;

#else

  if (dcl_fileexists(name))
    return fopen(name, "rb");

  return NULL;

#endif
}

FILE *open_global_config_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  char n[200];

  sprintf(n, CONFIGDIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, "r");

#endif

  return NULL;
}

FILE *open_local_config_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);
  if (dcl_fileexists(n))
    return fopen(n, "r");

  return NULL;

#else

  if (dcl_fileexists(name))
    return fopen(name, "r");

  return NULL;

#endif
}

FILE *create_local_config_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);

  return fopen(n, "w");

#else

  return fopen(name, "w");

#endif

}

FILE *open_highscore_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  char n[200];

  sprintf(n, HISCOREDIR"/%s", name);
  if (dcl_fileexists(n))
    return fopen(n, "rb");

  sprintf(n, "%s/.toppler/%s", homedir(), name);
  if (dcl_fileexists(n))
    return fopen(n, "rb");

  return NULL;

#else

  if (dcl_fileexists(name))
    return fopen(name, "rb");

  return NULL;

#endif

}

FILE *create_highscore_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);
  fclose(fopen(n, "a+"));
  return fopen(n, "r+");

#else

  fclose(fopen(name, "a+"));
  return fopen(name, "r+");

#endif
}


FILE *open_local_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);

  return fopen(n, "r");

#else

  return fopen(name, "rb");

#endif

}

FILE *create_local_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);
  return fopen(n, "w+");

#else

  fclose(fopen(name, "w+"));
  return fopen(name, "rb+");

#endif

}


static void parse_config(FILE * in) {
  char line[201], param[201];

  while (!feof(in)) {
    if (fscanf(in, "%200s%*[\t ]%200s\n", line, param) == 2) {
      for (int idx = 0; idx < SIZE(config_data); idx++) {
	if (strstr(line, config_data[idx].cnf_name)) {
	  switch (config_data[idx].cnf_typ) {
	    case CT_BOOL:
	      *(bool *)config_data[idx].cnf_var = str2bool(param);
	      break;
	    case CT_STRING:
	      if (strlen(param) > 1) {
		param[strlen(param)-1] = '\0';
		strncpy((char *)config_data[idx].cnf_var, param+1, config_data[idx].maxlen);
	      }
	      break;
	    case CT_INT:
	      *(int *)config_data[idx].cnf_var = atoi(param);
	      break;
	    case CT_KEY:
	      if (atoi(param) > 0)
		key_redefine((ttkey)config_data[idx].maxlen, (SDLKey)atoi(param));
	      break;
	    default: assert(0, "Unknown config data type.");
	  }
	  break;
	} //if (strstr(...
      } //for
    } //if (fscanf(...
  } //while (!feof(in))
}

void load_config(void) {
  fullscreen = false;
  nosound = false;

  FILE * in = open_global_config_file(".toppler.rc");
  
  if (in) {
    parse_config(in);
    fclose(in);
  }
  
  in = open_local_config_file(".toppler.rc");
  
  if (in) {
    parse_config(in);
    fclose(in);
  }
    
  if (start_lives < 1) start_lives = 1;
  else if (start_lives > 3) start_lives = 3;
}

void save_config(void) {
  FILE * out = create_local_config_file(".toppler.rc");

  if (out) {
    for (int idx = 0; idx < SIZE(config_data); idx++) {
	fprintf(out, "%s: ", config_data[idx].cnf_name);
	switch (config_data[idx].cnf_typ) {
	    case CT_BOOL: 
	      fprintf(out, "%s", (*(bool *)config_data[idx].cnf_var)?("yes"):("no"));
	      break;
	    case CT_STRING:
	      fprintf(out, "\"%s\"", (char *)(config_data[idx].cnf_var));
	      break;
	    case CT_INT:
	      fprintf(out, "%i", *(int *)config_data[idx].cnf_var);
	      break;
	    case CT_KEY:
	      fprintf(out, "%i", (int)key_conv2sdlkey((ttkey)config_data[idx].maxlen, true));
	      break;
	    default: assert(0, "Unknown config data type.");
	}
	fprintf(out, "\n");
    }
    fclose(out);
  }
}

static int sort_by_name(const void *a, const void *b) {
  return(strcmp(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name));
}

#if (SYSTEM == SYS_WINDOWS)

int alpha_scandir(const char *dir, struct dirent ***namelist,
            int (*select)(const struct dirent *)) {
  HANDLE hand;
  WIN32_FIND_DATA finddata;
  int i = 0;
  size_t entrysize;

  struct dirent entry;


  char name[200];

  sprintf(name, "%s\\*", dir);

  hand = FindFirstFile(name, &finddata);

  if (hand == INVALID_HANDLE_VALUE)
    return -1;

  *namelist = NULL;

  do {
    strncpy(entry.d_name, finddata.cFileName, 199);

    if (select == NULL || (select != NULL && (*select)(&entry)))
    {
      *namelist = (struct dirent **)realloc((void *)(*namelist), (size_t)((i + 1) * sizeof(struct dirent *)));
      if (*namelist == NULL)
        return(-1);
      entrysize = sizeof(struct dirent) - sizeof(entry.d_name) + strlen(entry.d_name) + 1;
      (*namelist)[i] = (struct dirent *)malloc(entrysize);
      if ((*namelist)[i] == NULL)
        return(-1);
      memcpy((*namelist)[i], &entry, entrysize);
      i++;
    }
  } while (FindNextFile(hand, &finddata));

  FindClose(hand);

  if (i == 0)
    return(-1);

  qsort((void *)(*namelist), (size_t)i, sizeof(struct dirent *), sort_by_name);

  return(i);
}

#else

int alpha_scandir(const char *dir, struct dirent ***namelist,
            int (*select)(const struct dirent *)) {
  DIR *d;
  struct dirent *entry;
  int i = 0;
  size_t entrysize;

  if ((d = opendir(dir)) == NULL)
     return(-1);

  *namelist = NULL;
  while ((entry = readdir(d)) != NULL)
  {
    if (select == NULL || (select != NULL && (*select)(entry)))
    {
      *namelist = (struct dirent **)realloc((void *)(*namelist), (size_t)((i + 1) * sizeof(struct dirent *)));
      if (*namelist == NULL)
        return(-1);
      entrysize = sizeof(struct dirent) - sizeof(entry->d_name) + strlen(entry->d_name) + 1;
      (*namelist)[i] = (struct dirent *)malloc(entrysize);
      if ((*namelist)[i] == NULL)
        return(-1);
      memcpy((*namelist)[i], entry, entrysize);
      i++;
    }
  }
  if (closedir(d))
    return(-1);

  if (i == 0)
    return(-1);

  qsort((void *)(*namelist), (size_t)i, sizeof(struct dirent *), sort_by_name);
    
  return(i);
}

#endif


