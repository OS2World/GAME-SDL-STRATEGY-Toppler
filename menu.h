#ifndef MENU_H
#define MENU_H

/* the menu and the higscores */

/* load graphics */
void men_init(void);

/* the main menu, return 0 for quit and
 bigger values for the missions */
unsigned char men_main(void);

/* highscores, after the game */
void men_highscore(unsigned long pt);

/* free graphics */
void men_done(void);

/* input line */
void men_input(char *s, int max_len, int ypos);


#endif
