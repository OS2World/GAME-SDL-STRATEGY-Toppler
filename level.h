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

#ifndef LEVEL_H
#define LEVEL_H

#include <SDL_types.h>

/* handles one mission with 8 towers and the necessary manipulations
 on the towerlayout when the game is going on */

/* lev_is_consistent() returns one of these */
#define TPROB_NONE            0 /* no problems found */
#define TPROB_NOSTARTSTEP     1 /* no starting step */
#define TPROB_STARTBLOCKED    2 /* starting position is blocked */
#define TPROB_UNDEFBLOCK      3 /* unknown block */
#define TPROB_NOELEVATORSTOP  4 /* elevator doesn't have stopping station(s) */
#define TPROB_ELEVATORBLOCKED 5 /* elevator is blocked */
#define TPROB_NOOTHERDOOR     6 /* door doesn't have opposing end */
#define TPROB_BROKENDOOR      7 /* door is not whole */
#define TPROB_NOEXIT          8 /* no exit doorway */
#define TPROB_UNREACHABLEEXIT 9 /* exit is unreachable */
#define NUM_TPROBLEMS        10

/* tries to find all missions installed on this system
 * returns the number of missions found
 */
void lev_findmissions();
Uint16 lev_missionnumber();
const char * lev_missionname(Uint16 num);

/* loads a mission from the file with the given name */
void lev_loadmission(Uint16 num);

/* free all the memory allocated by the mission and the mission list */
void lev_done();

/* returns the number of towers that are in the current mission */
Uint8 lev_towercount(void);

/* selects one of the towers in this mission */
void lev_selecttower(Uint8 number);

/* returns the color for the current tower */
Uint8 lev_towercol_red(void);
Uint8 lev_towercol_green(void);
Uint8 lev_towercol_blue(void);

void lev_set_towercol(Uint8 r, Uint8 g, Uint8 b);

/* returns the value at this position in the level array */
Uint8 lev_tower(Uint16 row, Uint8 column);

/* returns the height of the tower */
Uint8 lev_towerrows(void);

/* the name of the tower */
char *lev_towername(void);

/* the number of the actual tower */
Uint8 lev_towernr(void);

/* the time the player has to reach the top */
Uint16 lev_towertime(void);

void lev_set_towertime(Uint16 time);

/* removes one layer of the tower (for destruction) */
void lev_removelayer(Uint8 layer);

/* if the positions contains a vanishing step, remove it */
void lev_removevanishstep(int row, int col);

// returns true if the given position contains an upper end of a door
bool lev_is_door_upperend(int row, int col);

// returns true, if the given positions contains a level of a door
bool lev_is_door(int row, int col);

// returns true, if the given coordinates contains a layer of a target door
bool lev_is_targetdoor(int row, int col);

/* all for the elevators */

/* completely empty */
bool lev_is_empty(int row, int col);

/* contains a flahing box */
bool lev_is_box(int row, int col);

/* empty this field */
void lev_clear(int row, int col);

/* a station (not necessary with a platform */
bool lev_is_station(int row, int col);
bool lev_is_up_station(int row, int col);
bool lev_is_down_station(int row, int col);
bool lev_is_bottom_station(int row, int col);

/* contains a platform */
bool lev_is_platform(int row, int col);

/* conatins stick */
bool lev_is_stick(int row, int col);

/* id part of an elevator */
bool lev_is_elevator(int row, int col);

/* sliding step */
bool lev_is_sliding(int row, int col);

/* start and stop elevator */
void lev_platform2stick(int row, int col);
void lev_stick2platform(int row, int col);

/* move up and down */
void lev_stick2empty(int row, int col);
void lev_empty2stick(int row, int col);
void lev_platform2empty(int row, int col);

/* checks the given figure for validity of its position (can
 it be there without colliding ?) */
bool lev_testfigure(long angle, long vert, long back,
                    long fore, long typ, long height, long width);

/* checks the underground of the toppler */
int lev_testuntergr(int verticalpos, int anglepos, bool look_left);

/* used for the elevator */
unsigned char lev_putplatform(int row, int col);
void lev_restore(int row, int col, unsigned char bg);

/* --- the following commands are for the level editor ---  */

/* load and save a tower */
bool lev_loadtower(char *fname);
bool lev_savetower(char *fname);

/* rotate row clock and counter clockwise */
void lev_rotaterow(bool clockwise);

/* insert and delete one row */
void lev_insertrow(int position);
void lev_deleterow(int position);

/* creates a simple tower consisting of one empty row */
void lev_new(void);

/* functions to change on field on the tower */
void lev_putspace(int row, int col);
void lev_putrobot1(int row, int col);
void lev_putrobot2(int row, int col);
void lev_putrobot3(int row, int col);
void lev_putrobot4(int row, int col);
void lev_putrobot5(int row, int col);
void lev_putrobot6(int row, int col);
void lev_putrobot7(int row, int col);
void lev_putrobot8(int row, int col);
void lev_putstep(int row, int col);
void lev_putvanishingstep(int row, int col);
void lev_putslidingstep(int row, int col);
void lev_putdoor(int row, int col);
void lev_puttarget(int row, int col);
void lev_putstick(int row, int col);
void lev_putbox(int row, int col);
void lev_putelevator(int row, int col);
void lev_putmiddlestation(int row, int col);
void lev_puttopstation(int row, int col);

/* creates a copy of the current tower, the functions
 * allocate the necessary RAM and the restore function
 * frees the RAM again
 */
void lev_save(unsigned char *&data);
void lev_restore(unsigned char *&data);

/* check the tower for consistency. This function checks doors
 * and elevators if something is found row and col contain the
 * coordinates, and the return value is one of TPROB_xxx
 */
int lev_is_consistent(int &row, int &col);

/* mission creation: first call mission_new, then for each tower mission_addtower
 * finally to complete mission_finish, never use another calling order or you may
 * create corrupted mission files
 */
bool lev_mission_new(char * name);
void lev_mission_addtower(char * name);
void lev_mission_finish();

#endif

