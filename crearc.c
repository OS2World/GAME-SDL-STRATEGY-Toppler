#include <stdio.h>

#include <zlib.h>


typedef struct filenode {
  char name[8];
  long start, size, compressed;
} filenode;

int filesize(FILE *f) {
  long pos = ftell(f);
  int erg;
  fseek(f, 0, SEEK_END);
  erg = ftell(f);
  fseek(f, pos, SEEK_SET);
  return erg;
}

main(int argc, char *argv[])
{
  filenode files[256];
  int t, filecount, i;
  FILE *f;
  FILE *outf;
  unsigned char buffer[10000000];
  unsigned char buffer2[10000000];
  unsigned char buffer3[10000000];

  unsigned char compress_data[10000000];

  long erg;
  long err;

  filecount = 0;
  for (t = 1; t < argc; t++) {
    strcpy(files[filecount++].name, argv[t]);
  }

  outf = fopen("toppler.dat", "wb");

  fwrite(&filecount, 1, 1, outf);
  fwrite(files, sizeof(filenode) * filecount, 1, outf);

  for (t = 0; t < filecount; t++) {
    char name[200];
    sprintf(name, "%s.dat", files[t].name, files[t].name);
    f = fopen(name, "rb");

    files[t].size = filesize(f);
    files[t].start = ftell(outf);

    fread(buffer, files[t].size, 1, f);


    files[t].compressed = 10000000;
    compress(buffer2, &files[t].compressed, buffer, files[t].size);
    erg = 10000000;
    uncompress(buffer3, &erg, buffer2, files[t].compressed);

    if (erg != files[t].size)
      printf("different sizes (%i <-> %i", erg, files[t].size);

    for (i = 0; i < files[t].size; i++)
      if (buffer[i] != buffer3[i]) {
        printf("error in compression %i\n", i);
        break;
      }

    fwrite(buffer2, files[t].compressed, 1, outf);

    fclose(f);
  }
  rewind(outf);
  fwrite(&filecount, 1, 1, outf);
  fwrite(files, sizeof(filenode) * filecount, 1, outf);
  fclose(outf);
}


