#include "decl.h"

#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


bool fullscreen;
bool nosound;
bool doublescale;

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

/* checks if home/.toppler exists and creates it, if not */
static void checkdir(void) {
  char n[200];

  sprintf(n, "%s/.toppler", getenv("HOME"));

  DIR *d = opendir(n);

  if (!d) {
    mkdir(n, S_IRWXU);
  }
}

FILE *open_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  // look into actual directory
  if (dcl_fileexists(name))
    return fopen(name, "rb");

  // look into the data dir
  char n[200];

  sprintf(n, DATADIR"/%s", name);
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

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);
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

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);

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

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);
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

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);
  fclose(fopen(n, "a+"));
  return fopen(n, "r+");

#else

  return fopen(name, "r+");

#endif
}


FILE *open_local_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);

  return fopen(n, "r");

#else

  return fopen(name, "r");

#endif

}

FILE *create_local_data_file(char *name) {

#if (SYSTEM == SYS_LINUX)

  checkdir();

  char n[200];

  sprintf(n, "%s/.toppler/%s", getenv("HOME"), name);
  return fopen(n, "w+");

#else

  return fopen(name, "w");

#endif

}


static void parse_config(FILE * in) {
  char line[200];

  while (!feof(in)) {
    int i;
    fscanf(in, "%s %i\n", line, &i);

    if (strstr(line, "fullscreen")) {
      fullscreen = (i == 1);
    } else if (strstr(line, "nosound")){
      nosound = (i == 1);
    } else if (strstr(line, "scale2x")){
      doublescale = (i == 1);
    }
  }
}

void load_config(void) {
  fullscreen = false;
  nosound = false;
  doublescale = false;

  FILE * in = open_global_config_file(".toppler.rc");
  
  if (in)
    parse_config(in);
  
  in = open_local_config_file(".toppler.rc");
  
  if (in)
   parse_config(in);
}

void save_config(void) {
  FILE * out = create_local_config_file(".toppler.rc");

  if (out) {
    fprintf(out, "fullscreen: %i\n", (fullscreen)?(1):(0));
    fprintf(out, "nosound: %i\n", (nosound)?(1):(0));
    fprintf(out, "scale2x: %i\n", (doublescale)?(1):(0));
  
    fclose(out);
  }
}

#ifdef SELF_SCANDIR
int scandir(const char *dir, struct dirent ***namelist,
            int (*select)(const struct dirent *),
            int (*compar)(const void *, const void *)) {
  DIR *d;
  struct dirent *entry;
  register int i=0;
  size_t entrysize;

  if ((d=opendir(dir)) == NULL)
     return(-1);

  *namelist=NULL;
  while ((entry=readdir(d)) != NULL)
  {
    if (select == NULL || (select != NULL && (*select)(entry)))
    {
      *namelist=(struct dirent **)realloc((void *)(*namelist),
                 (size_t)((i+1)*sizeof(struct dirent *)));
        if (*namelist == NULL) return(-1);
        entrysize=sizeof(struct dirent)-sizeof(entry->d_name)+strlen(entry->d_name)+1;
        (*namelist)[i]=(struct dirent *)malloc(entrysize);
        if ((*namelist)[i] == NULL) return(-1);
        memcpy((*namelist)[i], entry, entrysize);
        i++;
    }
  }
  if (closedir(d)) return(-1);
  if (i == 0) return(-1);
  if (compar != NULL)
    qsort((void *)(*namelist), (size_t)i, sizeof(struct dirent *), compar);
    
  return(i);
}

int alphasort(const void *a, const void *b) {
  return(strcmp(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name));
}
#endif

