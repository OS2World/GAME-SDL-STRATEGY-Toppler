#ifndef SNOWBALL_H
#define SNOWBALL_H

/* updates and handles the snowball that can be thrown by
 the animal */

/* init, delete snowball */
void snb_init();

/* updates the position of the snowball */
void snb_movesnowball();

/* checks is there is already a snowball */
bool snb_exists();

/* creates a new one */
void snb_start(int verticalpos, int anglepos, bool look_left);

/* returns the position of the ball */
int snb_verticalpos();
int snb_anglepos();

#endif
