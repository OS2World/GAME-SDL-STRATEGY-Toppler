#include "level.h"

#include "points.h"
#include "archi.h"

#include <string.h>

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
  $91 10010001 vanishing steop
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

static unsigned char mission[4096];
static int towerheight;
static unsigned char tower[256][16];
static char towername[17];
static int towernumber;

void lev_loadmission(char *filename) {
  int res;

  arc_assign(filename);
  arc_read(mission, arc_filesize(), &res);
  arc_closefile();
}

int lev_selecttower(int number) {

  int towerstart;

  towernumber = number;

  // find start of towerdata in mission
  number *= 2;
  towerstart = mission[number + 1];
  towerstart = towerstart * 256 + mission[number];

  // extract towername
  int pos = 0;
  while (mission[towerstart + pos]) {
    towername[pos] = mission[towerstart + pos];
    pos++;
  }
  towername[pos] = 0;

  // save number of rows in tower
  towerheight = mission[towerstart + 16];

  // save start of bitmap and bytemap
  int bitstart = towerstart + 18;
  int bytestart = bitstart + mission[towerstart + 17];

  // initialize positions inside the fields
  int wpos = 0;
  int bpos = 0;

  // clean tower
  memset(&tower, 0, sizeof(char) * 2048L);

  // extract level data
  for (int row = 0; row < towerheight; row++)
    for (int col = 0; col < 16; col++) {
      if ((mission[bitstart + (bpos >> 3)] << (bpos & 7)) & 0x80)
        tower[row][col] = mission[bytestart + wpos++];
      else
        tower[row][col] = 0;
      bpos++;
  }

  return number * 50 + 500;
}


unsigned char lev_tower(int row, int column) {
  return tower[row][column];
}

int lev_towerheight() {
  return towerheight * 8;
}

char *lev_towername() {
  return towername;
}

int lev_towernr() {
  return towernumber;
}

void lev_removelayer(int layer) {
  while (layer < towerheight) {
    for (int c = 0; c < 16; c++)
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

/* returns true, if the given position is the upper end of e door
 (a door is always e layers */
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

int lev_towertime() { return  towernumber * 50 + 500; }

unsigned char lev_putplatform(int row, int col) {
  unsigned char erg = tower[row][col];

  tower[row][col] = 0x85;

  return erg;
}

void lev_restore(int row, int col, unsigned char bg) {
  tower[row][col] = bg;
}

