#include "keyb.h"

#include <SDL.h>

static Uint8 keydown, keytyped;
static char chartyped;

void key_init(void) {
  SDL_EnableKeyRepeat(0, 0);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
  SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

  keydown = keytyped = chartyped = 0;
}

static void handleEvents(void) {
  SDL_Event e;
  Uint8 key;

  while (SDL_PollEvent(&e)) {
    if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP)) {

      if (e.key.state == SDL_RELEASED) {
        if ((e.key.keysym.sym >= SDLK_a) && (e.key.keysym.sym <= SDLK_z))
          chartyped = e.key.keysym.sym - SDLK_a + 'A';
        if ((e.key.keysym.sym >= SDLK_0) && (e.key.keysym.sym <= SDLK_9))
          chartyped = e.key.keysym.sym - SDLK_0 + '0';
        if (e.key.keysym.sym == SDLK_SPACE)
          chartyped = ' ';
        if (e.key.keysym.sym == SDLK_RETURN)
          chartyped = 13;
        if (e.key.keysym.sym == SDLK_BACKSPACE)
          chartyped = 8;
        if (e.key.keysym.sym == SDLK_UP)
          chartyped = 1;
        if (e.key.keysym.sym == SDLK_DOWN)
          chartyped = 2;
        if (e.key.keysym.sym == SDLK_LEFT)
          chartyped = 3;
        if (e.key.keysym.sym == SDLK_RIGHT)
          chartyped = 4;
        if (e.key.keysym.sym == SDLK_ESCAPE)
          chartyped = 27;
        if (e.key.keysym.sym == SDLK_GREATER)
          chartyped = '>';
        if (e.key.keysym.sym == SDLK_EXCLAIM)
          chartyped = '!';
        if (e.key.keysym.sym == SDLK_HASH)
          chartyped = '#';
        if (e.key.keysym.sym == SDLK_MINUS)
          chartyped = '-';
        if (e.key.keysym.sym == SDLK_PERIOD)
          chartyped = '.';
        if (e.key.keysym.sym == SDLK_INSERT)
          chartyped = 5;
        if (e.key.keysym.sym == SDLK_DELETE)
          chartyped = 6;
        if (e.key.keysym.sym == SDLK_PAGEUP)
          chartyped = 7;
        if (e.key.keysym.sym == SDLK_PAGEDOWN)
          chartyped = 8;
        if (e.key.keysym.sym == SDLK_HOME)
          chartyped = 9;
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

void key_done(void) {
}

Uint8 key_keystat(void) {
  handleEvents();
  return keydown;
}

bool key_keypressed(Uint8 key) {
  handleEvents();
  return (keytyped & key);
}

Uint8 key_readkey(void) {
  handleEvents();

  int i = keytyped;

  keytyped = 0;
  chartyped = 0;

  return i;
}

char key_chartyped(void) {
  handleEvents();
  int erg = chartyped;
  chartyped = 0;
  return erg;
}

