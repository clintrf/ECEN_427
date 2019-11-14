
/*
* sound_state.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles, Seth Becerra
* BYU 2019
*/

#include <stdint.h>

/*********************************** macros ***********************************/

enum states {        // defines all of the state for the S.M.
  INIT_STATE,        // defines the init state for S.M.
  WALK01_STATE,        // defines the walk 1 state for S.M.
  WALK02_STATE,        // defines the walk 2 state for S.M.
  WALK03_STATE,        // defines the walk 3 state for S.M.
  WALK04_STATE,        // defines the walk 4 state for S.M.
  SAUCER_ZOOM_STATE,        // defines the saucer move state for S.M.
  SAUCER_EX_STATE,        // defines the saucer explode state for S.M.
  ALIEN_EX_STATE,        // defines the alien explode state for S.M.
  TANK_SHOOT_STATE,        // defines the shoot bullet state for S.M.
  TANK_EX_STATE,        // defines the tank explode state for S.M.
} current_state, return_state; // defines the current state for the S.M.

enum transitions {        // defines the transition states for the S.M.
  INIT_TRANS,        // defines the initialize state for the S.M.
  WALK01_TRANS,        // defines the transition walk 01 state for the S.M.
  WALK02_TRANS,        // defines the transition walk 2 state for the S.M.
  WALK03_TRANS,        // defines the transition walk 3 state for the S.M.
  WALK04_TRANS,        // defines the transition walk 4 state for the S.M.
  SAUCER_ZOOM_TRANS,  // defines the transition saucer move state for the S.M.
  SAUCER_EX_TRANS, // defines the transition saucer explode state for the S.M.
  ALIEN_EX_TRANS,    // defines the transition alien explode state for the S.M.
  TANK_SHOOT_TRANS,   // defines the transition shoot bullet state for the S.M.
  TANK_EX_TRANS,      // defines the transition tank explode state for the S.M.
} transition;

// funtion initializes S.M.
void sound_state_init();

// funtion for the state sound_state_machine
void sound_state_machine();
