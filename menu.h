#ifndef MENU_H
#define MENU_H

/* the menu and the higscores */

/* load graphics */
void men_init();

/* the main menu, return 0 for quit and
 bigger values for the missions */
unsigned char men_main(bool fade);

/* highscores, after the game */
void men_highscore(long pt, bool pal);

/* free graphics */
void men_done();

#endif
