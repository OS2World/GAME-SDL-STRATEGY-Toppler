#ifndef SOUND_H
#define SOUND_H

/* this module handles all the soundoutput necessary for the game */

/* load data */
void snd_init();

/* free data */
void snd_done();

/* play different sounds */

/* foot tapping */
void snd_tap();
/* elevator ticking */
void snd_tick();
/* door entering */
void snd_doortap();
/* cross appearing */
void snd_cross();
/* shooting */
void snd_shoot();
/* hit something with ball */
void snd_hit();
/* gloock gloock */
void snd_drown();
/* submarine is coming out of the water */
void snd_sub_raise();
/* submarine is giong into the water */
void snd_sub_down();
/* ball jumping and hitting something */
void snd_boink(int vol);
/* something falls into water */
void snd_splash(int vol);
/* time is running out */
void snd_alarm();
/* score */
void snd_score();
/* one layer crumbles */
void snd_crumble();
/* target door entered */
void snd_fanfare();
/* initial sound when the tower is shown */
void snd_start();
/* player sucks, timed out */
void snd_timeout();
/* toppler is falling */
void snd_fall();


void snd_play();

void snd_wateron();
void snd_wateroff();
void snd_watervolume(int v);

void snd_playtitle();
void snd_stoptitle();

#endif

