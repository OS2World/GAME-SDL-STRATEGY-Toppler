#ifndef ELEVATORS_H
#define ELEVATORS_H

#include <SDL_types.h>

/* this modules handles all the neccessities for up to 4
 elevators */

/* initializes the elevator structures */
void ele_init(void);

/* activates the elevator. there can be only one
 active elevator */
void ele_select(Uint8 row, Uint8 col);

/* the elevator gets startet */
void ele_activate(Sint8 dir);

/* moves the elevator one level up */
void ele_move(void);

/* return true, if the elevator is at a station */
bool ele_is_atstop(void);

/* the animal leaves the elevator or got thrown off.
 after a timeout the elevator will automatically fall down */
void ele_deactivate(void);

/* call once per update to check elevator falldown */
void ele_update(void);

#endif
