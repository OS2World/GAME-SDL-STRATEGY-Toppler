#ifndef ELEVATORS_H
#define ELEVATORS_H

/* this modules handles all the neccessities for up to 4
 elevators */

/* initializes the elevator structures */
void ele_init();

/* activates the elevator. there can be only one
 active elevator */
void ele_select(int row, int col);

/* the elevator gets startet */
void ele_activate(int dir);

/* moves the elevator one level up */
void ele_move();

/* return true, if the elevator is at a station */
bool ele_is_atstop();

/* the animal leaves the elevator or got thrown off.
 after a timeout the elevator will automatically fall down */
void ele_deactivate();

/* call once per update to check elevator falldown */
void ele_update();

#endif
