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

/* this module contains all file access functions necessary
 except the ones for the highscore file

 all the data is organized inside a small archive that contains a
 header defining the start and size of the different files */


#define FNAMELEN 12 /* 8.3 filename */

/* forward declaration */
class file;


/*
 * this class handles one archive, each archive can contain any number of
 * files with names of up to FNAMELEN characters to open one file use assign
 * and use the returned file class
 */

class archive {

public:
  /* opens the archive, you must give the FILE handle to the
   * file that is the archive to this constructor
   */
  archive(FILE *file);

  /* closes the archive */
  ~archive();

  /* opens one file inside the archive */
  file * assign(char *name);

private:

  FILE *f;
  
  typedef struct {
    char name[FNAMELEN];
    Uint32 start, size, compress;
  } fileindex;
  
  fileindex *files;
  Uint8 filecount;
};



/*
 * this class is used to handle the access to the different files inside
 * the archive(s) internally it only handles one bich chunk of memory
 * so take care that your files do not grow beyond all limits and
 * do not forget to delete the files, otherwis you will have a memory
 * leak
 */

class file {

public:

  /* close the file and free memory */
  ~file();

  /* returns the size of the currently opened file */
  Uint32 size(void);

  /* returns true if the current file is completely read */
  bool eof(void);

  /* reads up to size bytes into the buffer, returning in result
   the real number read */
  Uint32 read(void *buf, Uint32 size);

  /* read one byte from currently opened file */
  Uint8 getbyte(void);

  /* read one word from currently opened file this read is endian save */
  Uint16 getword(void);

private:

  Uint8* buffer;
  Uint32 fsize, bufferpos;

  file(Uint8 *buf, Uint32 sz) : buffer(buf), fsize(sz), bufferpos(0) {};

  friend file * archive::assign(char *name);

};

extern archive dataarchive;

#endif

