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

#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pwd.h>
#include <dirent.h>

bool fullscreen;
bool nosound;
bool doublescale;
bool use_water = true;
bool use_alpha = true;

char editor_towername[TOWERNAMELEN+1] = "";

void dcl_wait(void) {
  static Uint32 last;
  while ((SDL_GetTicks() - last) < 55*1 ) SDL_Delay(2);
  last = SDL_GetTicks();
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

  fclose(fopen(n, "a+"));
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

  return fopen(name, "r");

#endif

}

FILE *create_local_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", homedir(), name);
  return fopen(n, "w+");

#else

  return fopen(name, "w");

#endif

}


static void parse_config(FILE * in) {
  char line[200], param[200];

  while (!feof(in)) {
    fscanf(in, "%s %s\n", line, param);

    if (strstr(line, "fullscreen")) {
      fullscreen = (atoi(param) == 1);
    } else if (strstr(line, "scale2x")) {
      doublescale = (atoi(param) == 1);
    } else if (strstr(line, "nosound")) {
      nosound = (atoi(param) == 1);
    } else if (strstr(line, "editor_towername")) {
      int len = strlen(param);
      if (len <= TOWERNAMELEN) {
        memcpy(editor_towername, param, strlen(param));
        editor_towername[len] = '\0';
      } else editor_towername[0] = '\0';
    }
  }
}

void load_config(void) {
  fullscreen = false;
  nosound = false;
  doublescale = false;

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
}

void save_config(void) {
  FILE * out = create_local_config_file(".toppler.rc");

  if (out) {
    fprintf(out, "fullscreen: %i\n", (fullscreen)?(1):(0));
    fprintf(out, "scale2x: %i\n", (doublescale)?(1):(0));
     fprintf(out, "nosound: %i\n", (nosound)?(1):(0));
     fprintf(out, "editor_towername: %s\n", editor_towername);
  
    fclose(out);
  }
}

static int sort_by_name(const void *a, const void *b) {
  return(strcmp(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name));
}

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


