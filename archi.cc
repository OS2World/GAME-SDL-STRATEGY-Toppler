#include "archi.h"

#include "decl.h"

#include <SDL_endian.h>

#include <string.h>
#include <stdlib.h>

#define MAX_FILES 9

static FILE *f;

static Uint32 bitbuffer;
static Uint8 bitpos;

typedef struct {
  char name[16];
  Uint32 start, size;
} fileindex;

static fileindex files[MAX_FILES];
static unsigned char filecount;
static Uint8 pos;


void arc_init(char *name) {
  f = open_data_file(name);

  assert(f != 0, "archive file could not be opened\n");

  fread(&filecount, 1, 1, f);
  assert(filecount <= MAX_FILES, "too many files in archive");

  for (Uint8 file = 0; file < filecount; file++) {
    for (Uint8 strpos = 0; strpos < 16; strpos++)
      fread(&files[file].name[strpos], 1, 1, f);


    Uint8 tmp;

    /* load start from archive */
    fread(&tmp, 1, 1, f);
    files[file].start = (Uint32(tmp)) << 0;
    fread(&tmp, 1, 1, f);
    files[file].start |= (Uint32(tmp)) << 8;
    fread(&tmp, 1, 1, f);
    files[file].start |= (Uint32(tmp)) << 16;
    fread(&tmp, 1, 1, f);
    files[file].start |= (Uint32(tmp)) << 24;

    /* load filesize from archive */
    fread(&tmp, 1, 1, f);
    files[file].size = (Uint32(tmp)) << 0;
    fread(&tmp, 1, 1, f);
    files[file].size |= (Uint32(tmp)) << 8;
    fread(&tmp, 1, 1, f);
    files[file].size |= (Uint32(tmp)) << 16;
    fread(&tmp, 1, 1, f);
    files[file].size |= (Uint32(tmp)) << 24;
  }
}

void arc_closefile(void) {
}

void arc_done(void) {
  arc_closefile();

  fclose(f);
}

void arc_assign(char *name) {

  assert(f != 0, "archive not initialized\n");

  for (Uint8 i = 0; i < filecount; i++)
    if (strcmp(name, files[i].name) == 0) {
      fseek(f, files[i].start, SEEK_SET);
      bitpos = 0;
      pos = i;
      return;
    }

  assert(0, "file not found in archive\n");

}


void arc_read(void *buf, Uint32 size, Uint32 *result) {
  *result = fread(buf, 1, size, f);
}

static Uint8 getbyte(void) {
  Uint8 erg;

  fread(&erg, 1, 1, f);

  return erg;
}

Uint16 arc_getbits(Uint8 anz) {
  Uint16 result;

  while (bitpos <= 24) {
    bitbuffer = (bitbuffer << 8) | getbyte();
    bitpos += 8;
  }

  result = (bitbuffer >> (bitpos - anz)) & ((1 << anz) - 1);
  bitpos -= anz;
  return result;
}

Uint32 arc_filesize(void) {
  return files[pos].size;
}

bool arc_eof(void) {
  return ftell(f) > files[pos].start + files[pos].size;
}

