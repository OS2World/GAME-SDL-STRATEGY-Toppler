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

#ifndef SOUND_H
#define SOUND_H

/* this module handles all the soundoutput necessary for the game */

/* load data */
void snd_init(void);

/* free data */
void snd_done(void);

/* play different sounds */

/* foot tapping */
void snd_tap(void);
/* elevator ticking */
void snd_tick(void);
/* door entering */
void snd_doortap(void);
/* cross appearing */
void snd_cross(void);
/* shooting */
void snd_shoot(void);
/* hit something with ball */
void snd_hit(void);
/* gloock gloock */
void snd_drown(void);
/* submarine is coming out of the water */
void snd_sub_raise(void);
/* submarine is going into the water */
void snd_sub_down(void);
/* ball jumping and hitting something */
void snd_boink(int vol);
/* something falls into water */
void snd_splash(int vol);
/* time is running out */
void snd_alarm(void);
/* score */
void snd_score(void);
/* one layer crumbles */
void snd_crumble(void);
/* target door entered */
void snd_fanfare(void);
/* initial sound when the tower is shown */
void snd_start(void);
/* player sucks, timed out */
void snd_timeout(void);
/* toppler is falling */
void snd_fall(void);


void snd_play(void);

void snd_wateron(void);
void snd_wateroff(void);
void snd_watervolume(int v);

void snd_playtitle(void);
void snd_stoptitle(void);
void snd_playtgame(void);
void snd_stoptgame(void);

#endif

