#ifndef toppler_h
#define toppler_h

/* this modules handles the the green animal, updates its position
 on the tower and its shape corresponding to the shape of the tower */


/* initisalizes the variables, call this function each time a
 you start at the tower. */
void top_init();

/* actualizes the position and sgape of the toppler considering the given keyposition */
void top_aktualtoppler(int left_right, int up_down, bool space);

/* tests, if the toppler collides with something while it is on the elevator */
void top_testkollision();

/* the following functions return serveral of the necesary variables for the toppler */

/* its vertial position on the tower */
int top_verticalpos();

/* the angle position on the tower */
int top_anglepos();

/* is it visible */
bool top_visible();

/* does it look left (or right) */
bool top_look_left();

/* the shape, independent of the direction */
int top_shape();

/* is it on an elevator */
bool top_onelevator();

/* technice bonus points, how often got it thrown down */
int top_technic();

/* the actual state of the toppler */

/* drowned */
bool top_died();

/* reached target */
bool top_targetreached();

/* the game ended, either drowned or reached target */
bool top_ended();

/* the animal is currently drowning */
bool top_dying();

/* it is moving */
bool top_walking();

/* needed for destruction of tower, to drop the toppler
 one layer of the tower */
void top_drop1layer();

/* hide the toppler */
void top_hide();

/* show it and set its shape vertical and angular position */
void top_show(int shape, int vpos, int apos);

/* move the toppler to the side until it is at a valid position
 this function is necessary for the downfalling elevators to
 push the animal aside  */
void top_sidemove();


#endif
