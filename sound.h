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
/* submarine is giong into the water */
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

#endif

