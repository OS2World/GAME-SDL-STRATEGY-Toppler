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

#include <SDL.h>

/*
 * this modules contains the code for the level editor
 */

/* Editor key actions. 
   If you add here, change _ed_key_actions[] in leveledit.cc */
#define EDACT_QUIT      0
#define EDACT_MOVEUP    1
#define EDACT_MOVEDOWN  2
#define EDACT_MOVELEFT  3
#define EDACT_MOVERIGHT 4
#define EDACT_INSROW    5
#define EDACT_DELROW    6
#define EDACT_ROT180    7
#define EDACT_PUTSPACE  8
#define EDACT_PUTSTEP   9
#define EDACT_PUTVANISHER 10
#define EDACT_PUTSLIDER   11
#define EDACT_PUTDOOR     12
#define EDACT_PUTGOAL     13
#define EDACT_CHECKTOWER  14
#define EDACT_PUTROBOT1   15
#define EDACT_PUTROBOT2   16
#define EDACT_PUTROBOT3   17
#define EDACT_PUTROBOT4   18
#define EDACT_PUTROBOT5   19
#define EDACT_PUTROBOT6   20
#define EDACT_PUTROBOT7   21
#define EDACT_PUTLIFT     22
#define EDACT_PUTLIFTMID  23
#define EDACT_PUTLIFTTOP  24
#define EDACT_PUTSTICK    25
#define EDACT_PUTBOX      26
#define EDACT_LOADTOWER   27
#define EDACT_SAVETOWER   28
#define EDACT_TESTTOWER   29
#define EDACT_SETTOWERCOLOR 30
#define EDACT_INCTIME       31
#define EDACT_DECTIME       32
#define EDACT_CREATEMISSION 33
#define EDACT_MOVEPAGEUP    34
#define EDACT_MOVEPAGEDOWN  35
#define EDACT_GOTOSTART     36
#define EDACT_SHOWKEYHELP   37
#define EDACT_NAMETOWER     38
#define EDACT_SETTIME       39

#define NUMEDITORACTIONS    40

struct _ed_key {
   int action;
   SDLKey key;
};

#define TOWERPAGESIZE 5 /* pageup/pagedown moving */
#define TOWERSTARTHEI 4 /* tower starting height */

void le_edit(void);

