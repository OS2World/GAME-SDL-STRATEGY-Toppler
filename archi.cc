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


#include "archi.h"

#include "decl.h"

#include <SDL_endian.h>

#include <string.h>
#include <stdlib.h>

#if (SYSTEM == SYS_WINDOWS)

#define _WINDOWS
#define ZLIB_DLL

#endif

#include <zlib.h>

#define FNAMELEN 12 /* 8.3 filename */

static FILE *f;

static Uint32 bitbuffer;
static Uint8 bitpos;

typedef struct {
  char name[FNAMELEN];
  Uint32 start, size, compress;
} fileindex;

static fileindex *files;
static Uint8 filecount;
static Uint8 pos;

static Uint8 * buffer = 0;
static Uint32 bufferpos;


void arc_init(char *name) {
  f = open_data_file(name);

  assert(f != 0, "archive file could not be opened\n");

  fread(&filecount, 1, 1, f);
    
  files = (fileindex *)malloc(filecount * sizeof(fileindex));
  assert(files, "Failed to alloc memory\n");

  for (Uint8 file = 0; file < filecount; file++) {
    for (Uint8 strpos = 0; strpos < FNAMELEN; strpos++)
      fread(&files[file].name[strpos], 1, 1, f);

    Uint8 tmp;

    /* load start from archive */
    fread(&tmp, 1, 1, f);
    files[file].start = ((Uint32)(tmp)) << 0;
    fread(&tmp, 1, 1, f);
    files[file].start |= ((Uint32)(tmp)) << 8;
    fread(&tmp, 1, 1, f);
    files[file].start |= ((Uint32)(tmp)) << 16;
    fread(&tmp, 1, 1, f);
    files[file].start |= ((Uint32)(tmp)) << 24;

    /* load filesize from archive */
    fread(&tmp, 1, 1, f);
    files[file].size = ((Uint32)(tmp)) << 0;
    fread(&tmp, 1, 1, f);
    files[file].size |= ((Uint32)(tmp)) << 8;
    fread(&tmp, 1, 1, f);
    files[file].size |= ((Uint32)(tmp)) << 16;
    fread(&tmp, 1, 1, f);
    files[file].size |= ((Uint32)(tmp)) << 24;

    /* load compressed size from archive */
    fread(&tmp, 1, 1, f);
    files[file].compress = ((Uint32)(tmp)) << 0;
    fread(&tmp, 1, 1, f);
    files[file].compress |= ((Uint32)(tmp)) << 8;
    fread(&tmp, 1, 1, f);
    files[file].compress |= ((Uint32)(tmp)) << 16;
    fread(&tmp, 1, 1, f);
    files[file].compress |= ((Uint32)(tmp)) << 24;
  }
}

void arc_closefile(void) {
  if (buffer) {
    delete [] buffer;
    buffer = 0;
  }
}

void arc_done(void) {
  arc_closefile();

  free(files);
    
  fclose(f);
}

void arc_assign(char *name) {

  assert(f, "archive not initialized\n");

  arc_closefile();

  for (Uint8 i = 0; i < filecount; i++)
    if (strncmp(name, files[i].name, FNAMELEN) == 0) {

      buffer = new Uint8[files[i].size];
      Uint8 * b = new Uint8[files[i].compress];

      unsigned long erg;

      fseek(f, files[i].start, SEEK_SET);
      fread(b, files[i].compress, 1, f);
      erg = files[i].size;
      uncompress(buffer, &erg, b, files[i].compress);

      assert(erg == files[i].size, "data file corrupt\n");

      delete [] b;
      bitpos = 0;
      pos = i;
      bufferpos = 0;
      return;
    }

  assert(0, "file not found in archive\n");

}

void arc_read(void *buf, Uint32 size, Uint32 *result) {
  memcpy(buf, &buffer[bufferpos], size);
  bufferpos += size;
  *result = size;
}

static Uint8 getbyte(void) {
  return buffer[bufferpos++];
}

Uint16 arc_getbits(Uint8 anz) {
  Uint16 result;

  while (bitpos < anz) {
    bitbuffer = (bitbuffer << 8) | getbyte();
    bitpos += 8;
  }

  result = (bitbuffer >> (bitpos - anz)) & ((1 << anz) - 1);
  bitpos -= anz;
  return result;
}

void arc_bytealign(void) {
  bitpos = (bitpos + 7) & ~7;
}

Uint32 arc_filesize(void) {
  return files[pos].size;
}

bool arc_eof(void) {
  return bufferpos >= files[pos].size;
}

