
#ifndef TXTSYS_H
#define TXTSYS_H

#include <SDL.h>
#include "menu.h"

class textsystem {

public:
  textsystem(char *title, menuopt_callback_proc pr);
  ~textsystem();
  void addline(char *line);
  void run();

private:
  void draw();

  char *title;
  int  numlines;                  // # of lines
  int  shownlines;                // # of lines shown on screen
  int  ystart;                    // screen y coord where text starts
  long max_length;                // how long is the longest line?
  char **lines;
  menuopt_callback_proc mproc;    // background drawing proc
  long xoffs;                     // current x offset
  long yoffs;                     // current y offset
  long disp_xoffs;                // displayed x offset
  long disp_yoffs;                // displayed y offset
};

#endif /* TXTSYS_H */
