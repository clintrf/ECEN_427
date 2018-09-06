/*
* sound_state.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>

/*********************************** macros ***********************************/

enum states {
  INIT_STATE,
  WALK01_STATE,
  WALK02_STATE,
  WALK03_STATE,
  WALK04_STATE,
  SAUCER_ZOOM_STATE,
  SAUCER_EX_STATE,
  ALIEN_EX_STATE,
  TANK_SHOOT_STATE,
  TANK_EX_STATE,
} current_state, return_state;

enum transitions {
  INIT_TRANS,
  WALK01_TRANS,
  WALK02_TRANS,
  WALK03_TRANS,
  WALK04_TRANS,
  SAUCER_ZOOM_TRANS,
  SAUCER_EX_TRANS,
  ALIEN_EX_TRANS,
  TANK_SHOOT_TRANS,
  TANK_EX_TRANS,
} transition;

// funtion initializes state machine
void sound_state_init();

// funtion for the state sound_state_machine
void sound_state_machine();

