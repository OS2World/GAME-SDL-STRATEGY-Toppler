#include "keyb.h"

#include <SDL/SDL.h>

static int keydown, keytyped;
static char chartyped;

void key_init() {
  SDL_EnableKeyRepeat(0, 0);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

  keydown = keytyped = chartyped = 0;
}

static void handleEvents() {
  SDL_Event e;
  int key;

  while (SDL_PollEvent(&e)) {
    if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP)) {

      if (e.key.state == SDL_RELEASED) {
        if ((e.key.keysym.sym >= SDLK_a) && (e.key.keysym.sym <= SDLK_z))
          chartyped = e.key.keysym.sym - SDLK_a + 'A';
        if (e.key.keysym.sym == SDLK_SPACE)
          chartyped = ' ';
        if (e.key.keysym.sym == SDLK_RETURN)
          chartyped = 13;
        if (e.key.keysym.sym == SDLK_BACKSPACE)
          chartyped = 8;
      }


      switch (e.key.keysym.sym) {
        case SDLK_SPACE:
        case SDLK_RETURN:
          key = fire_key;
          break;
        case SDLK_UP:
          key = up_key;
          break;
        case SDLK_DOWN:
          key = down_key;
          break;
        case SDLK_LEFT:
          key = left_key;
          break;
        case SDLK_RIGHT:
          key = right_key;
          break;
        case SDLK_ESCAPE:
          key = break_key;
          break;
        case SDLK_p:
          key = pause_key;
          break;
        default:
          key = 0;
      }
      if (e.key.state == SDL_PRESSED) {
        keydown |= key;
        keytyped |= key;
      } else {
        keydown &= ~key;
      }
    }
  }
}

void key_done() {
}

int key_keystat() {
  handleEvents();
  return keydown;
}

bool key_keypressed(int key) {
  handleEvents();
  return (keytyped & key);
}

char key_readkey() {
  handleEvents();

  int i = keytyped;

  keytyped = 0;
  chartyped = 0;

  return i;
}

char key_chartyped() {
  handleEvents();
  int erg = chartyped;
  chartyped = 0;
  return erg;
}

