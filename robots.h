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

#ifndef ROBOTS_H
#define ROBOTS_H

/* this module handles the movement of up to 4 robots */


/* values for kinds of robots */
#define OBJ_KIND_NOTHING 0
#define OBJ_KIND_JUMPBALL 1
#define OBJ_KIND_FREEZEBALL 2
#define OBJ_KIND_FREEZEBALL_FROZEN 5
#define OBJ_KIND_FREEZEBALL_FALLING 3
#define OBJ_KIND_DISAPPEAR 4
#define OBJ_KIND_APPEAR 6
#define OBJ_KIND_CROSS 7
#define OBJ_KIND_ROBOT_VERT 8
#define OBJ_KIND_ROBOT_HORIZ 9

/* initialize all fields, call this when you start a new towergame*/
void rob_initialize(void);

/* return the position and state of one robot */
int rob_kind(int nr);
int rob_time(int nr);
int rob_angle(int nr);
int rob_vertical(int nr);

/* returns the object the snowball or animal collides with or -1 */
int rob_topplerkollision(int angle, int vertical);
int rob_snowballkollision(int angle, int vertical);

/* creates new robots, depending on the actual vertical position and
 the actual number of robots existing */
void rob_new(int verticalpos);

/* move all the robots */
void rob_aktualize(void);

/* call this if a robot got hit my the snowball, the function
 returns the number of points the player gets */
int rob_gothit(int nr);

/* makes all the robots diappear */
void rob_disappearall(void);

#endif
