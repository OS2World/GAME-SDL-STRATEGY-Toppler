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

#if 0

static FILE *f;

typedef struct {
  char name[FNAMELEN];
  Uint32 start, size, compress;
} fileindex;

static fileindex *files;
static Uint8 filecount;
static Uint8 pos;

static Uint8 *buffer = 0;
static Uint32 bufferpos;

void arc_init(char *name) {
  f = open_data_file(name);

  assert(f != 0, "Archive file could not be opened.");

  fread(&filecount, 1, 1, f);

  files = new fileindex[filecount];
  assert(files, "Failed to alloc memory for archive index.");

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

  delete [] files;
    
  fclose(f);
}

void arc_assign(char *name) {

  assert(f, "Archive not initialized.");

  arc_closefile();

  for (Uint8 i = 0; i < filecount; i++)
    if (strncmp(name, files[i].name, FNAMELEN) == 0) {

      buffer = new Uint8[files[i].size];
      Uint8 *b = new Uint8[files[i].compress];

      unsigned long erg;

      fseek(f, files[i].start, SEEK_SET);
      fread(b, files[i].compress, 1, f);
      erg = files[i].size;
      uncompress(buffer, &erg, b, files[i].compress);

      assert(erg == files[i].size, "Data file corrupt.");

      delete [] b;
      pos = i;
      bufferpos = 0;
      return;
    }

  assert(0, "File not found in archive!");
}

void arc_read(void *buf, Uint32 size, Uint32 *result) {
  memcpy(buf, &buffer[bufferpos], size);
  bufferpos += size;
  *result = size;
}

Uint8 arc_getbyte(void) {
  return buffer[bufferpos++];
}

Uint16 arc_getword(void) {
  Uint16 w = (Uint16)buffer[bufferpos] + ((Uint16)buffer[bufferpos+1] << 8);
  bufferpos+=2;
  return w;
}

Uint32 arc_filesize(void) {
  return files[pos].size;
}

bool arc_eof(void) {
  return bufferpos >= files[pos].size;
}

#endif

archive::archive(FILE *stream) {

  f = stream;

  fread(&filecount, 1, 1, f);

  files = new fileindex[filecount];

  assert(files, "Failed to alloc memory for archive index.");

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

archive::~archive() {
  delete [] files;
  fclose(f);
}

file * archive::assign(char *name) {

  for (Uint8 i = 0; i < filecount; i++)
    if (strncmp(name, files[i].name, FNAMELEN) == 0) {

      Uint8 *buffer = new Uint8[files[i].size];
      Uint8 *b = new Uint8[files[i].compress];

      unsigned long erg;

      fseek(f, files[i].start, SEEK_SET);
      fread(b, files[i].compress, 1, f);
      erg = files[i].size;
      uncompress(buffer, &erg, b, files[i].compress);

      assert(erg == files[i].size, "Data file corrupt.");

      delete [] b;
      return new file(buffer, files[i].size);
    }

  assert(0, "File not found in archive!");
}

file::~file() { delete(buffer); }

Uint32 file::size(void) { return fsize; }

bool file::eof(void) {
  return bufferpos >= fsize;
}

Uint32 file::read(void *buf, Uint32 size) {
  memcpy(buf, &buffer[bufferpos], size);
  bufferpos += size;
  return size;
}

Uint8 file::getbyte(void) {
  return buffer[bufferpos++];
}

Uint16 file::getword(void) {
  Uint16 w = (Uint16)buffer[bufferpos] + ((Uint16)buffer[bufferpos+1] << 8);
  bufferpos+=2;
  return w;
}

