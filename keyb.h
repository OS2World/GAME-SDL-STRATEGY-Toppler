#ifndef KEYB_H
#define KEYB_H

/* keyboard handling */

#define no_key          0
#define up_key          1
#define down_key        2
#define left_key        4
#define right_key       8
#define fire_key        16
#define break_key       32
#define pause_key       64
#define any_key         255


void key_init(void);
void key_done(void);

/* returns bitmask with currently pressed keys */
int key_keystat(void);

/* true, if key is pressed */
bool key_keypressed(int key);

/* returns if a key has been pushed and released (typed) but only for the keys in
 the list */
char key_readkey(void);

/* returns a types character */
char key_chartyped(void);

#endif

