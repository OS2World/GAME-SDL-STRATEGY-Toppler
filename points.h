#ifndef POINTS_H
#define POINTS_H

/* handles points and lifes */

/* retest points to 0 and lifes to 4 */
void pts_reset();

/* ass points, updating the life counter, if necesary */
void pts_add(int add);

/* returns the current points and lifes */
unsigned int pts_points();
unsigned char pts_lifes();

/* removes one live */
void pts_died();

/* returns true, if lifes != 0 */
bool pts_lifesleft();

#endif
