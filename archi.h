#ifndef ARCHI_H
#define ARCHI_H

#include <SDL.h>

/* this module contains all file acces functions necessary
 except the ones for the highscore file

 all the data is organized inside a small archive that contains a
 header defining the start and size of the different files */

/* opens the archve, the given name is the filename of
 the archive, no checks are done */
void arc_init(char *name);

/* closes the archive */
void arc_done();

/* opens one file inside the archive */
void arc_assign(char *name);

/* closes one opened file in the archive */
void arc_closefile();

/* returns the size of the currently opened file */
long arc_filesize();

/* returns true if the current file is completely read */
bool arc_eof();

/* reads up to size bytes into the buffer, returning in result
 the real number read */
void arc_read(void *buf, int size, int *result);

/* returns the specified number of bits on the lower bits of the retuned
 value, dont use arc_read and arc_getbits on the same file */
unsigned short arc_getbits(int anz);

/* returns a SDL_RWops for the currently opened file */
SDL_RWops *arc_getRWops();

#endif

