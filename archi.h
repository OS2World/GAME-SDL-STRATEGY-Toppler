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
void arc_done(void);

/* opens one file inside the archive */
void arc_assign(char *name);

/* closes one opened file in the archive */
void arc_closefile(void);

/* returns the size of the currently opened file */
Uint32 arc_filesize(void);

/* returns true if the current file is completely read */
bool arc_eof(void);

/* reads up to size bytes into the buffer, returning in result
 the real number read */
void arc_read(void *buf, Uint32 size, Uint32 *result);

/* returns the specified number of bits on the lower bits of the retuned
 value, dont use arc_read and arc_getbits on the same file */
Uint16 arc_getbits(Uint8 anz);


/* drops the rest of the bits in the currently started byte */
void arc_bytealign(void);

#endif

