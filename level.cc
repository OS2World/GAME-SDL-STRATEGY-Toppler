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

#include "level.h"

#include "points.h"
#include "archi.h"
#include "decl.h"

#include <string.h>
#include <stdlib.h>


/*
  bitmasks for the levelfield

  0 == empty

  $08 00001000 top station (marker)
  $0c 00001100 middle station (marker)
  $04 00000100 bottom station (marker)

  $10 00010000 robot
  $20          robot
  $30          robot
  $40          robot
  $50          robot
  $60          robot
  $70          robot
  $40          robot

  $80 10000000 stick (normal)
  $81 10000001 step
  $82 10000010 box
  $83 10000011 normal door
  $84 10000100 stick (at the bottom station)
  $85 10000101 platform (at the bottom station)
  $88 10001000 stick (at the top station) (not used)
  $89 10001001 platform (at the top station)
  $8c 10001100 stick (at middle station)
  $8d 10001101 platform (at middle station)
  $91 10010001 vanishing step
  $B1 10110001 sliding step
  $C3 11000011 target door
      ||||||||
      |||||||+--\ 0 = stick, 1 = step or platform, 2 = box, 3 = door
      |||||||   >
      ||||||+---/
      ||||||
      |||||+----\ 
      |||||     > elevator one bit for each possible movin direction
      ||||+-----/
      ||||
      |||+------\
      |||       > modifier for steps 1 = disappering, 3 = sliding, 2 = unused
      ||+-------/
      ||
      |+-------- = 1 means targetdoor
      |
      +--------- = 1 means the corresponding field contains a tower element

*/

#define TOWERWID 16

static Uint8 * mission = NULL;
static Uint8 towerheight;
static Uint8 tower[256][TOWERWID];
static char towername[TOWERNAMELEN+1];
static Uint8 towernumber;
static Uint8 towercolor_red, towercolor_green, towercolor_blue;
static Uint16 towertime;

struct _towercharconv {
   Uint8 dat;
   char  ch;
} static towerchar_conv[] = {
     {0x00, ' '},
     {0x10, '1'},
     {0x20, '2'},
     {0x30, '3'},
     {0x40, '4'},
     {0x50, '5'},
     {0x60, '6'},
     {0x70, '7'},
     {0x80, '!'},
     {0x81, '-'},
     {0x82, 'b'},
     {0x83, '#'},
     {0xc3, 'T'},
     {0x85, '^'},
     {0x08, 'v'},
     {0x0c, '+'},
     {0x91, '.'},
     {0xb1, '>'},
};

typedef struct mission_node {
  char name[30];
  char fname[100];
  mission_node *next;
} mission_node;

mission_node * missions;

static int missionfiles (const struct dirent *file)
{
  int len = strlen(file->d_name);

  return ((file->d_name[len - 1] == 'm') &&
          (file->d_name[len - 2] == 't') &&
          (file->d_name[len - 3] == 't') &&
          (file->d_name[len - 4] == '.'));
}


Uint8 conv_char2towercode(char ch) {
  for (int x = 0; x < SIZE(towerchar_conv); x++)
    if (ch == towerchar_conv[x].ch) return towerchar_conv[x].dat;
  return 0x00;
}

char conv_towercode2char(Uint8 code) {
  for (int x = 0; x < SIZE(towerchar_conv); x++)
    if (code == towerchar_conv[x].dat) return towerchar_conv[x].ch;
  return ' ';
}

static void add_mission(char *fname) {

  char mname[30];

  FILE * f = fopen(fname, "rb");

  if (!f) return;

  unsigned char mnamelength;
  fread(&mnamelength, 1, 1, f);

  if (mnamelength > 29) mnamelength = 29;

  fread(mname, mnamelength, 1, f);
  mname[mnamelength] = 0;

  mission_node * m = missions;
  mission_node * l = NULL;

  while (m) {

    int erg = strcmp(m->name, mname);
    /* no two missions with the same name */
    if (erg == 0)
      return;

    /* we have passed your target, the current mission must
     * be inserted bevore this mission
     */
    if (erg > 0) {
      mission_node * n = new mission_node;
      strcpy(n->name, mname);
      strcpy(n->fname, fname);
      n->next = m;

      if (l)
        l->next = n;
      else
        missions = n;

      return;
    }

    l = m;
    m = m->next;
  }

  /* insert at the end */
  m = new mission_node;
  strcpy(m->name, mname);
  strcpy(m->fname, fname);
  m->next = NULL;

  if (l)
    l->next = m;
  else
    missions = m;

  fclose(f);
}


void lev_findmissions() {

  char pathname[100];

  struct dirent **eps = NULL;

  missions = NULL;

  /* check if already called, if so free the old list */
  while (missions) {
    mission_node *n = missions;
    missions = missions->next;
    delete n;
  }

#if SYSTEM == SYS_WINDOWS
  {
    char n[100];
    GetCurrentDirectory(100, n);
    sprintf(pathname, "%s\\", n);
  }
#else
  sprintf(pathname, "%s", "./");
#endif

  int n = scandir(pathname, &eps, missionfiles, alphasort);

  if (n >= 0) {

    for (int i = 0; i < n; i++) {

      char fname[200];
      sprintf(fname, "%s%s", pathname, eps[i]->d_name);

      add_mission(fname);
    }
  }
  free(eps);
  eps = NULL;

  sprintf(pathname, "%s/.toppler/", getenv("HOME"));
  n = scandir(pathname, &eps, missionfiles, alphasort);

  if (n >= 0) {

    for (int i = 0; i < n; i++) {

      char fname[200];
      sprintf(fname, "%s%s", pathname, eps[i]->d_name);

      add_mission(fname);
    }
  }
  free(eps);
  eps = NULL;

  sprintf(pathname, "%s/", TOP_DATADIR);
  n = scandir(pathname, &eps, missionfiles, alphasort);

  if (n >= 0) {

    for (int i = 0; i < n; i++) {

      char fname[200];
      sprintf(fname, "%s%s", pathname, eps[i]->d_name);

      add_mission(fname);
    }
  }
  free(eps);
  eps = NULL;
}

void lev_done() {
  if (mission) {
    delete [] mission;
    mission = NULL;
  }

  mission_node * m = missions;

  while (m) {
    m = m->next;
    delete missions;
    missions = m;
  }
}


Uint16 lev_missionnumber() {
  int num = 0;
  mission_node * m = missions;

  while (m) {
    num++;
    m = m->next;
  }

  return num;
}

const char * lev_missionname(Uint16 num) {
  mission_node * m = missions;

  while (num) {
    m = m->next;
    num--;
  }

  return m->name;
}

void lev_loadmission(Uint16 num) {

  mission_node *m = missions;
  while (num) {
    num--;
    m = m->next;
  }

  FILE * in = fopen(m->fname, "rb");

  if (mission) delete [] mission;

  /* find out file size */
  fseek(in, 0, SEEK_END);
  int fsize = ftell(in);

  /* get enough memory and load the whole file into memory */
  mission = new unsigned char[fsize];
  fseek(in, 0, SEEK_SET);
  fread(mission, fsize, 1, in);

  fclose(in);
}

Uint8 lev_towercount(void) {
  return mission[mission[0] + 1];
}

void lev_selecttower(Uint8 number) {

  Uint32 towerstart;

  towernumber = number;

  // find start of towerdata in mission
  {
    Uint32 idxpos = 0;

    idxpos += mission[mission[0] + 2];
    idxpos += long(mission[mission[0] + 3]) << 8;
    idxpos += long(mission[mission[0] + 4]) << 16;
    idxpos += long(mission[mission[0] + 5]) << 24;


    towerstart = mission[idxpos + 4 * number];
    towerstart += long(mission[idxpos + 4 * number + 1]) << 8;
    towerstart += long(mission[idxpos + 4 * number + 2]) << 16;
    towerstart += long(mission[idxpos + 4 * number + 3]) << 24;
  }
   
  // extract towername
  memmove(towername, &mission[towerstart + 1], mission[towerstart]);
  towername[mission[towerstart]] = 0;

  towerstart += mission[towerstart] + 1;

  // save number of rows in tower
  towerheight = mission[towerstart++];
  towertime = mission[towerstart] + (int(mission[towerstart + 1]) << 8);
  towerstart += 2;

  towercolor_red = mission[towerstart];
  towercolor_green = mission[towerstart + 1];
  towercolor_blue = mission[towerstart + 2];
  towerstart += 3;

  // save start of bitmap and bytemap
  Uint32 bitstart = towerstart;
  Uint32 bytestart = bitstart + 2 * towerheight;

  // initialize positions inside the fields
  Uint16 wpos = 0;
  Uint16 bpos = 0;

  // clean tower
  memset(&tower, 0, 2048L);

  // extract level data
  for (Uint8 row = 0; row < towerheight; row++)
    for (Uint8 col = 0; col < TOWERWID; col++) {
      if ((mission[bitstart + (bpos >> 3)] << (bpos & 7)) & 0x80)
        tower[row][col] = mission[bytestart + wpos++];
      else
        tower[row][col] = 0;
      bpos++;
    }
}

void lev_set_towercol(Uint8 r, Uint8 g, Uint8 b) {
  towercolor_red = r;
  towercolor_green = g;
  towercolor_blue = b;
}

Uint8 lev_towercol_red() {
  return towercolor_red;
}

Uint8 lev_towercol_green() {
  return towercolor_green;
}

Uint8 lev_towercol_blue() {
  return towercolor_blue;
}

unsigned char lev_tower(Uint8 row, Uint8 column) {
  return tower[row][column];
}

Uint8 lev_towerrows(void) {
  return towerheight;
}

const char * lev_towername(void) {
  return towername;
}

Uint8 lev_towernr(void) {
  return towernumber;
}

Uint16 lev_towertime(void) {
  return towertime;
}

void lev_set_towertime(Uint16 time) {
  towertime = time;
}

void lev_removelayer(Uint8 layer) {
  while (layer < towerheight) {
    for (Uint8 c = 0; c < TOWERWID; c++)
      tower[layer][c] = tower[layer + 1][c];
    layer++;
  }

  towerheight--;
}

/* empties a cell in the tower */
void lev_clear(int row, int col) {
  tower[row][col] = 0;
}


/* if the given position contains a vanishing step, remove it */
void lev_removevanishstep(int row, int col) {
  if (tower[row][col] == 0x91)
    tower[row][col] = 0;
}

/********** everything for doors ********/

/* returns true, if the given position is the upper end of a door
 (a door is always 3 layers) */
bool lev_is_door_upperend(int row, int col) {
  return lev_is_door(row, col) &&
    lev_is_door(row + 1, col) &&
    lev_is_door(row + 2, col);
}

/* returns true if the given position contains a door */
bool lev_is_door(int row, int col) {
  return (tower[row][col] & 0x83) == 0x83;
}

/* returns true, if the given fiels contains a target door */
bool lev_is_targetdoor(int row, int col) {
  return tower[row][col] == 0xc3;
}

/**************** everything for elevators ******************/

bool lev_is_station(int row, int col) {
  return (tower[row][col] & 0x0c);
}
bool lev_is_up_station(int row, int col) {
  return (tower[row][col] & 0x85) == 0x85;
}
bool lev_is_down_station(int row, int col) {
  return (tower[row][col] & 0x89) == 0x89;
}
bool lev_is_bottom_station(int row, int col) {
  return (tower[row][col] & 0x8d) == 0x85;
}

bool lev_is_platform(int row, int col) {
  return (tower[row][col] & 0x83) == 0x81;
}
bool lev_is_stick(int row, int col) {
  return (tower[row][col] & 0x83) == 0x80;
}

bool lev_is_elevator(int row, int col) {
  return ((tower[row][col] & 0x80) &&
    (tower[row][col] & 0x0c));
}

void lev_platform2stick(int row, int col) {
  tower[row][col] &= 0xfe;
}
void lev_stick2platform(int row, int col) {
  tower[row][col] |= 0x01;
}
void lev_stick2empty(int row, int col) {
  tower[row][col] &= ~0x80;
}
void lev_empty2stick(int row, int col) {
  tower[row][col] |= 0x80;
}
void lev_platform2empty(int row, int col) {
  tower[row][col] &= ~0x83;
}

/* misc questions */
bool lev_is_empty(int row, int col) {
  return (tower[row][col] & 0x80) == 0;
}

bool lev_is_box(int row, int col) {
  return tower[row][col] == 0x82;
}

bool lev_is_sliding(int row, int col) {
  return (tower[row][col] & 0x30) == 0x30;
}

/* returns true, if the given figure can be at the given position
 without kolliding with fixed objects of the tower */
bool lev_testfigure(long angle, long vert, long back,
                    long fore, long typ, long height, long width) {
  long hinten, vorn, y, x = 0, k, t;

  hinten = ((angle + back) >> 3) & 0xf;
  vorn = (((angle + fore) >> 3) + 1) & 0xf;

  y = vert / 4;
  vert &= 3;

  switch (typ) {

    case 0:  /* toppler */
      x = (vert == 3) ? 3 : 2;
      break;
  
    case 1:  /* robot */
      x = (vert == 0) ? 1 : 2;
      break;
  
    case 2:  /* snowball */
      x = (vert == 0) ? 0 : 1;
      break;
  }

  do {
    k = x;
    do {
      if (lev_is_platform(k + y, hinten)) {
        return false;
      } else if (lev_is_stick(k + y, hinten)) {
        t = hinten * 8 + height;
        if ((angle >= t) && (angle < t + width))
          return false;
      } else if (lev_is_box(k + y, hinten)) {
        t = hinten * 8 + height;
        if ((angle >= t) && (angle < t + width)) {
          if (typ == 2) {
            // the snowball removes the box
            lev_clear(k + y, hinten);
            pts_add(50);
          }
          return false;
        }
      }
      k--;
    } while (k != -1);
    hinten = (hinten + 1) & 0xf;
  } while (hinten != vorn);

  return true;
}

/* tests the underground of the animal at the given position returning
 0 if everything is all right
 1 if there is no undergound below us (fall vertial)
 2 if there is no undergound behind us fall backwards
 3 if there is no underfound in front of us (fall forwards) */
int lev_testuntergr(int verticalpos, int anglepos, bool look_left) {
  static unsigned char unter[32] = {
    0x11, 0x20, 0x02, 0x00,
    0x11, 0x00, 0x32, 0x00,
    0x11, 0x00, 0x32, 0x00,
    0x11, 0x00, 0x11, 0x00,
    0x11, 0x00, 0x11, 0x00,
    0x11, 0x00, 0x11, 0x00,
    0x11, 0x23, 0x00, 0x00,
    0x11, 0x23, 0x00, 0x00
  };

  int erg;

  int r = (verticalpos / 4) - 1;
  int c = ((anglepos + 0x7a) / 8) & 0xf;

  erg = (lev_is_empty(r, c) || lev_is_door(r, c)) ? 0 : 2;

  c = ((anglepos + 0x7a) / 8 + 1) & 0xf;

  if ((!lev_is_empty(r, c)) && (!lev_is_door(r, c))) erg++;

  erg = unter[(anglepos & 0x7) * 4 + erg];

  if (look_left)
    return erg >> 4;
  else
    return erg & 0xf;
}

unsigned char lev_putplatform(int row, int col) {
  unsigned char erg = tower[row][col];

  tower[row][col] = 0x85;

  return erg;
}

void lev_restore(int row, int col, unsigned char bg) {
  tower[row][col] = bg;
}


/* load and save a tower */
bool lev_loadtower(char *fname) {
  FILE *in = open_local_data_file(fname);

  if (in == NULL) return false;

  fgets(towername, TOWERNAMELEN+1, in);
  fscanf(in, "%hhu, %hhu, %hhu\n", &towercolor_red, &towercolor_green, &towercolor_blue);

  fscanf(in, "%hu\n", &towertime);
  fscanf(in, "%hhu\n", &towerheight);

  for (int row = towerheight - 1; row >= 0; row--) {
    char line[200];

    fgets(line, 200, in);

    for (int col = 0; col < TOWERWID; col++)
      tower[row][col] = conv_char2towercode(line[col]);
  }

  fclose(in);
  return true;
}

bool lev_savetower(char *fname) {
  FILE *out = create_local_data_file(fname);

  if (out == NULL) return false;

  fprintf(out, "%s\n", towername);
  fprintf(out, "%hhu, %hhu, %hhu\n", towercolor_red, towercolor_green, towercolor_blue);
  fprintf(out, "%hu\n", towertime);
  fprintf(out, "%hhu\n", towerheight);

  for (int row = towerheight - 1; row >= 0; row--) {
    char line[TOWERWID+2];

    for (int col = 0; col < TOWERWID; col++)
      line[col] = conv_towercode2char(tower[row][col]);

    line[TOWERWID] = '|';
    line[TOWERWID+1] = 0;
    fprintf(out, "%s\n", line);
  }

  fclose(out);

  return true;
}

/* rotate row clock and counter clockwise */
void lev_rotaterow(int row, bool clockwise) {
  if (clockwise) {
    int k = tower[row][0];
    for (int i = 1; i < TOWERWID; i++)
      tower[row][i - 1] = tower[row][i];
    tower[row][TOWERWID-1] = k;
  } else {
    int k = tower[row][TOWERWID-1];
    for (int i = TOWERWID-1; i >= 0; i++)
      tower[row][i] = tower[row][i - 1];
    tower[row][0] = k;
  }
}

/* insert and delete one row */
void lev_insertrow(int position) {
  if ((towerheight < 255) && (position < towerheight)) {
    int k = towerheight - 1;
    while (k >= position) {
      for (int i = 0; i < TOWERWID; i++)
        tower[k + 1][i] = tower[k][i];
      k--;
    }
    for (int i = 0; i < TOWERWID; i++)
      tower[position][i] = 0;
    towerheight++;
    return;
  }
  if (towerheight == 0) {
    for (int i = 0; i < TOWERWID; i++)
      tower[0][i] = 0;
    towerheight = 1;
  }
}

void lev_deleterow(int position) {
  if ((position < towerheight) && (position >= 0)) {
    int k = position + 1;
    while (k < towerheight) {
      for (int i = 0; i < TOWERWID; i++)
        tower[k - 1][i] = tower[k][i];
      k++;
    }
    towerheight--;
  }
}

void lev_new(void) {
  towerheight = 1;

  for (int i = 0; i < TOWERWID; i++)
    tower[0][i] = 0;
}

void lev_putspace(int row, int col) {

  // always delete the whole door
  if (lev_is_door(row, col)) {
    int r = row - 1;
    while (lev_is_door(r, col)) {
      tower[r][col] = 0x00;
      r--;
    }
    r = row + 1;
    while (lev_is_door(r, col)) {
      tower[r][col] = 0x00;
      r++;
    }
  }
  tower[row][col] = 0x00;
}
void lev_putrobot1(int row, int col) { tower[row][col] = 0x10; }
void lev_putrobot2(int row, int col) { tower[row][col] = 0x20; }
void lev_putrobot3(int row, int col) { tower[row][col] = 0x30; }
void lev_putrobot4(int row, int col) { tower[row][col] = 0x40; }
void lev_putrobot5(int row, int col) { tower[row][col] = 0x50; }
void lev_putrobot6(int row, int col) { tower[row][col] = 0x60; }
void lev_putrobot7(int row, int col) { tower[row][col] = 0x70; }
void lev_putstep(int row, int col) { tower[row][col] = 0x81; }
void lev_putvanishingstep(int row, int col) { tower[row][col] = 0x91; }
void lev_putslidingstep(int row, int col) { tower[row][col] = 0xb1; }

void lev_putdoor(int row, int col) {

  if (row + 2 < towerheight) {

    tower[row][col] = 0x83;
    tower[row + 1][col] = 0x83;
    tower[row + 2][col] = 0x83;

    if ((tower[row][(col + 8) & 0xf] == 0) &&
        (tower[row + 1][(col + 8) & 0xf] == 0) &&
        (tower[row + 2][(col + 8) & 0xf] == 0)) {
      tower[row][(col + 8) & 0xf] = 0x83;
      tower[row + 1][(col + 8) & 0xf] = 0x83;
      tower[row + 2][(col + 8) & 0xf] = 0x83;
    }
  }
}

void lev_puttarget(int row, int col) {
  if (row + 2 < towerheight) {
    tower[row][col] = 0xc3;
    tower[row + 1][col] = 0xc3;
    tower[row + 2][col] = 0xc3;
  }
}

void lev_putstick(int row, int col) { tower[row][col] = 0x80; }
void lev_putbox(int row, int col) { tower[row][col] = 0x82; }
void lev_putelevator(int row, int col) { tower[row][col] = 0x85; }
void lev_putmiddlestation(int row, int col) { tower[row][col] = 0x0c; }
void lev_puttopstation(int row, int col) { tower[row][col] = 0x08; }


void lev_save(unsigned char *&data) {
  data = new unsigned char[256*TOWERWID+1];

  data[0] = towerheight;
  memmove(&data[1], tower, 256 * TOWERWID);
}

void lev_restore(unsigned char *&data) {
  memmove(tower, &data[1], 256 * TOWERWID);
  towerheight = data[0];

  delete [] data;
}

int lev_is_consistent(int &row, int &col) {

   int y;
   bool has_exit = false;
  // check first, if the starting point is correctly organized
  // so that there is no obstacle and we can survive there
  if (((tower[1][0] != 0x80) && (tower[1][0] != 0x81) &&
       (tower[1][0] != 0x82) && (tower[1][0] != 0x85) &&
       (tower[1][0] != 0xb1) && (tower[0][0] != 0x80) && 
       (tower[0][0] != 0x81) && (tower[0][0] != 0x82) && 
       (tower[0][0] != 0x85) && (tower[0][0] != 0xb1))) {
	row = 1;
	col = 0;
	return TPROB_NOSTARTSTEP;
  }
  for (y = 2; y < 5; y++)
     if ((tower[y][0] >= 0x10) && (tower[y][0] != 0xc3) &&
	 (tower[y][0] != 0x83)) {
	row = y;
	col = 0;
	return TPROB_STARTBLOCKED;
     }

  for (int r = 0; r < towerheight; r++)
    for (int c = 0; c < TOWERWID; c++) {

      // check for undefined symbols
      if ((tower[r][c] != 0x00) &&
          (tower[r][c] != 0x10) &&
          (tower[r][c] != 0x20) &&
          (tower[r][c] != 0x30) &&
          (tower[r][c] != 0x40) &&
          (tower[r][c] != 0x50) &&
          (tower[r][c] != 0x60) &&
          (tower[r][c] != 0x70) &&
          (tower[r][c] != 0x80) &&
          (tower[r][c] != 0x81) &&
          (tower[r][c] != 0x82) &&
          (tower[r][c] != 0x83) &&
          (tower[r][c] != 0xc3) &&
          (tower[r][c] != 0x85) &&
          (tower[r][c] != 0x08) &&
          (tower[r][c] != 0x0c) &&
          (tower[r][c] != 0x91) &&
          (tower[r][c] != 0xb1) &&
          (tower[r][c] != 0x00) &&
          (tower[r][c] != 0x00) &&
          (tower[r][c] != 0x00)) {
        row = r;
        col = c;
        return TPROB_UNDEFBLOCK;
      }

      // check if elevators always have an opposing end without unremovable
      // obstacles
      if (tower[r][c] == 0x85) {
        int d = r + 1;
        while ((tower[d][c] != 0x08) && (d < towerheight)) {
          if ((tower[d][c] != 0x00) &&
              (tower[d][c] != 0x10) &&
              (tower[d][c] != 0x20) &&
              (tower[d][c] != 0x30) &&
              (tower[d][c] != 0x40) &&
              (tower[d][c] != 0x50) &&
              (tower[d][c] != 0x60) &&
              (tower[d][c] != 0x70) &&
              (tower[d][c] != 0x82) &&
              (tower[d][c] != 0x0c) &&
              (tower[d][c] != 0x91)) {
            row = r;
            col = c;
            return TPROB_ELEVATORBLOCKED;
          }
          d++;
        }
        if (d >= towerheight) {
          row = r;
          col = c;
          return TPROB_NOELEVATORSTOP;
        }
      }

      if (tower[r][c] == 0x0c) {
        int d = r + 1;
        while ((tower[d][c] != 0x08) && (d < towerheight)) {
          if ((tower[d][c] != 0x00) &&
              (tower[d][c] != 0x10) &&
              (tower[d][c] != 0x20) &&
              (tower[d][c] != 0x30) &&
              (tower[d][c] != 0x40) &&
              (tower[d][c] != 0x50) &&
              (tower[d][c] != 0x60) &&
              (tower[d][c] != 0x70) &&
              (tower[d][c] != 0x82) &&
              (tower[d][c] != 0x0c) &&
              (tower[d][c] != 0x91)) {
            row = r;
            col = c;
            return TPROB_ELEVATORBLOCKED;
          }
          d++;
        }
        if (d >= towerheight) {
          row = r;
          col = c;
          return TPROB_NOELEVATORSTOP;
        }
        d = r - 1;
        while ((tower[d][c] != 0x85) && (d >= 0)) {
          if ((tower[d][c] != 0x00) &&
              (tower[d][c] != 0x10) &&
              (tower[d][c] != 0x20) &&
              (tower[d][c] != 0x30) &&
              (tower[d][c] != 0x40) &&
              (tower[d][c] != 0x50) &&
              (tower[d][c] != 0x60) &&
              (tower[d][c] != 0x70) &&
              (tower[d][c] != 0x82) &&
              (tower[d][c] != 0x0c) &&
              (tower[d][c] != 0x91)) {
            row = r;
            col = c;
            return TPROB_ELEVATORBLOCKED;
          }
          d--;
        }
        if (d < 0) {
          row = r;
          col = c;
          return TPROB_NOELEVATORSTOP;
        }
      }

      if (tower[r][c] == 0x08) {
        int d = r - 1;
        while ((tower[d][c] != 0x85) && (d >= 0)) {
          if ((tower[d][c] != 0x00) &&
              (tower[d][c] != 0x10) &&
              (tower[d][c] != 0x20) &&
              (tower[d][c] != 0x30) &&
              (tower[d][c] != 0x40) &&
              (tower[d][c] != 0x50) &&
              (tower[d][c] != 0x60) &&
              (tower[d][c] != 0x70) &&
              (tower[d][c] != 0x82) &&
              (tower[d][c] != 0x0c) &&
              (tower[d][c] != 0x91)) {
            row = r;
            col = c;
            return TPROB_ELEVATORBLOCKED;
          }
          d--;
        }
        if (d < 0) {
          row = r;
          col = c;
          return TPROB_NOELEVATORSTOP;
        }
      }

      /* check for exit, and that it's reachable */
      if (tower[r][c] == 0xc3) {
	 int d = r - 1;
	 while ((d >= 0) && (tower[d][c] == 0xc3))  d--;
	 if (d >= 0) {
	    if ((tower[d][c] != 0x80) && (tower[d][c] != 0x81) &&
		(tower[d][c] != 0x82) && (tower[d][c] != 0x85) &&
		(tower[d][c] != 0x80)) {
	       row = r;
	       col = c;
	       return TPROB_UNREACHABLEEXIT;
	    }
	 }
	 has_exit = true;
      }
      
      // check doors
      if ((tower[r][c] == 0x83) &&
          ((tower[r][(c + (TOWERWID/2)) % TOWERWID] & 0x83) != 0x83)) {
        row = r;
        col = c;
        return TPROB_NOOTHERDOOR;
      }
      if ((tower[r][c] & 0x83) == 0x83) {
        bool A = (r > 0) && (tower[r-1][c] == tower[r][c]);
        bool B = (r > 1) && (tower[r-2][c] == tower[r][c]);
        bool D = (r + 1 < towerheight) && (tower[r+1][c] == tower[r][c]);
        bool E = (r + 2 < towerheight) && (tower[r+2][c] == tower[r][c]);

        if (!(A&&B||A&&D||D&&E)) {
          row = r;
          col = c;
          return TPROB_BROKENDOOR;
        }
      }
    }

  if (!has_exit) return TPROB_NOEXIT;
  return TPROB_NONE;
}

/* the functions for mission creation */

static FILE * fmission = NULL;
static Uint8 nmission = 0;
static Uint32 missionidx[256];

bool lev_mission_new(char * name) {
  assert(!fmission, "called mission_finish twice");

  char fname[200];
  sprintf(fname, "%s.ttm", name);

  fmission = create_local_data_file(fname);

  if (!fmission) return false;

  unsigned char tmp = strlen(name);

  /* write out name */
  fwrite(&tmp, 1 ,1, fmission);
  fwrite(name, 1, tmp, fmission);

  /* placeholders for towernumber and indexstart */
  fwrite(&tmp, 1, 1, fmission);
  fwrite(&tmp, 1, 4, fmission);

  nmission = 0;

  return true;
}

void lev_mission_addtower(char * name) {
  assert(fmission, "called mission_addtower without mission_new");

  Uint8 rows, col;
  Sint16 row;

  FILE * in = open_local_data_file(name);
  if (!tower) return;

  missionidx[nmission] = ftell(fmission);
  nmission++;

  {
    char towername[TOWERNAMELEN+1];
    fgets(towername, TOWERNAMELEN+1, in);

    Uint8 tmp = strlen(towername);
    fwrite(&tmp, 1, 1, fmission);
    fwrite(towername, 1, tmp, fmission);
  }

  {
    Uint8 red, green, blue, towerheight, tmp;
    Uint16 towertime;

    fscanf(in, "%hhu, %hhu, %hhu\n", &red, &green, &blue);
    fscanf(in, "%hu\n", &towertime);
    fscanf(in, "%hhu\n", &towerheight);

    fwrite(&towerheight, 1, 1, fmission);
    tmp = towertime & 0xff;
    fwrite(&tmp, 1, 1, fmission);
    tmp = (towertime >> 8) & 0xff;
    fwrite(&tmp, 1, 1, fmission);

    fwrite(&red, 1, 1, fmission);
    fwrite(&green, 1, 1, fmission);
    fwrite(&blue, 1, 1, fmission);

    rows = towerheight;
  }

  /* load the tower */
  Uint8 tower[256][TOWERWID];
  for (row = rows - 1; row >= 0; row--) {
    char line[200];

    fgets(line, 200, in);

    for (Uint8 col = 0; col < TOWERWID; col++)
       tower[row][col] = conv_char2towercode(line[col]);
  }

  /* output bitmap */
  for (row = 0; row < rows; row++) {

    Uint8 c = 0;
    for (col = 0; col < 8; col ++)
      if (tower[row][col])
        c |= (0x80 >> col);

    fwrite(&c, 1, 1, fmission);

    c = 0;
    for (col = 0; col < 8; col ++)
      if (tower[row][col + 8])
        c |= (0x80 >> col);

    fwrite(&c, 1, 1, fmission);
  }

  /* output bytemap */
  for (row = 0; row < rows; row++)
    for (col = 0; col < TOWERWID; col++)
      if (tower[row][col])
        fwrite(&tower[row][col], 1, 1, fmission);

  fclose(in);
}

void lev_mission_finish() {
  assert(fmission, "called mission_finish without mission_new");

  Uint8 c;

  /* save indexstart and write out index */
  Uint32 idxpos = ftell(fmission);
  for (Uint8 i = 0; i < nmission; i++) {

    c = missionidx[i] & 0xff;
    fwrite(&c, 1, 1, fmission);
    c = (missionidx[i] >> 8) & 0xff;
    fwrite(&c, 1, 1, fmission);
    c = (missionidx[i] >> 16) & 0xff;
    fwrite(&c, 1, 1, fmission);
    c = (missionidx[i] >> 24) & 0xff;
    fwrite(&c, 1, 1, fmission);
  }

  /* write out the number of towers in this mission */
  fseek(fmission, 0, SEEK_SET);
  fread(&c, 1, 1, fmission);
  printf("%i\n", c);

  fseek(fmission, c + 1, SEEK_SET);
  fwrite(&nmission, 1, 1, fmission);

  /* write out index position */
  c = idxpos & 0xff;
  fwrite(&c, 1, 1, fmission);
  c = (idxpos >> 8) & 0xff;
  fwrite(&c, 1, 1, fmission);
  c = (idxpos >> 16) & 0xff;
  fwrite(&c, 1, 1, fmission);
  c = (idxpos >> 24) & 0xff;
  fwrite(&c, 1, 1, fmission);

  fclose(fmission);

  fmission = NULL;
}

