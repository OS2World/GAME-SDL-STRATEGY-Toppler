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
void snd_door();
/* cross appearing */
void snd_cross();
/* shooting */
void snd_shoot();
/* hit something with ball */
void snd_hit();
/* gloock gloock */
void snd_drown();
void snd_sub_raise();
void snd_sub_down();
/* ball jumping and hitting something */
void snd_boink(int vol);
/* something falls into water */
void snd_splash(int vol);

void snd_play();

void snd_wateron();
void snd_wateroff();
void snd_watervolume(int v);



#endif

