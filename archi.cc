#include "archi.h"

#include "decl.h"

#include <string.h>
#include <stdlib.h>

static long pos;
static FILE *f;

static unsigned long bitbuffer;
static int bitpos;

static struct {
  char name[13];
  long start, size;
} files[9];


void arc_init(char *name) {
  f = file_open(name, "rb");

  assert(f != 0, "archive file not opened\n");

  fread(files, 1, 1, f);
  fread(files, sizeof(files), 1, f);
}

void arc_closefile()
{
}

void arc_done()
{
  arc_closefile();

  fclose(f);
}

void arc_assign(char *name) {

  assert(f != 0, "archive not initialized\n");

  for (int i = 0; i < 9; i++)
    if (strcmp(name, files[i].name) == 0) {
      fseek(f, files[i].start, SEEK_SET);
      bitpos = 0;
      pos = i;
      return;
    }

  assert(0, "file not found in archive\n");

}


void arc_read(void *buf, int size, int *result) {

  *result = fread(buf, 1, size, f);
}

static unsigned char getbyte() {
  unsigned char erg;
  int res;

  res = fread(&erg, 1, 1, f);

  return erg;
}

unsigned short arc_getbits(int anz) {
  unsigned short result;

  while (bitpos <= 24) {
    bitbuffer = (bitbuffer << 8) | getbyte();
    bitpos += 8;
  }

  result = (bitbuffer >> (bitpos - anz)) & ((1 << anz) - 1);
  bitpos -= anz;
  return result;
}

long arc_filesize() {
  return files[pos].size;
}

bool arc_eof() {
  return ftell(f) > files[pos].start + files[pos].size;
}

SDL_RWops *arc_getRWops() {
}


