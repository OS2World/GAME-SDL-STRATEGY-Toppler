#ifndef LEVEL_H
#define LEVEL_H

/* handles one mission with 8 towers and the necessary manipulations
 on the towerlayout when the game is going on */

/* loads a mission from the file with the given name */
void lev_loadmission(char *filename);

/* selects one of the towers in this mission */
int lev_selecttower(int number);

/* returns the value at this position in the level array */
unsigned char lev_tower(int row, int column);

/* returns the height of the tower */
int lev_towerheight();

/* the name of the tower */
char *lev_towername();

/* the number of the actual tower */
int lev_towernr();

/* the time the player has to reach the top */
int lev_towertime();

/* removes one layer of the tower (for destruction) */
void lev_removelayer(int layer);

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


#endif

