#include "globals.h"

/********************************** macros ***********************************/
#define RIGHT 0
#define LEFT 1
#define STARTING_LEFT_COLUMN 0
#define STARTING_RIGHT_COLUMN 10
#define STARTING_ALIEN_AMOUNT 55
#define ALIEN_ALIVE 1
#define NOT_FIRED 0
#define FIRED 1

/********************************** globals **********************************/
static uint16_t aliens_alive[STARTING_ALIEN_AMOUNT] = {ALIEN_ALIVE};
static uint16_t aliens_movement = RIGHT;
static uint16_t alien_block_far_left = STARTING_LEFT_COLUMN;
static uint16_t alien_block_far_right = STARTING_RIGHT_COLUMN;
static uint16_t tank_bullet_fired = NOT_FIRED;
static uint32_t tank_bullet_position;

/********************************* functions *********************************/
// fetch whether the tank bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_tank_bullet_fired() {
  return tank_bullet_fired;
}

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the top of the screen
void globals_fire_tank_bullet() {
  tank_bullet_fired = FIRED;
}

// set this to 0 once the bullet hits a target or reaches the top of the screen, prevents the tank from firing more bullets
void globals_tank_bullet_stopped() {
  tank_bullet_fired = NOT_FIRED;
}

// fetch the current tank bullet position
// returns : the current tank position
uint32_t globals_get_tank_bullet_position() {
  return tank_bullet_position;
}

// set a new tank bullet position
// pos : the new position which you wish to set
void globals_set_tank_bullet_position(uint32_t pos) {
  tank_bullet_position = pos;
}
