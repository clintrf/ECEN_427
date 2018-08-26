#include "globals.h"

/********************************** macros ***********************************/
#define STARTING_LEFT_COLUMN 0
#define STARTING_RIGHT_COLUMN 10
#define STARTING_ALIEN_AMOUNT 55
#define ALIEN_ALIVE 1
#define NOT_FIRED 0
#define FIRED 1
#define SAUCER_ALIVE 1
#define SAUCER_SHOT 0

/********************************** globals **********************************/
static uint16_t tank_bullet_fired = NOT_FIRED;
static uint32_t tank_bullet_position;
static uint32_t current_score = 0;
static uint32_t saucer_pos = GLOBALS_SAUCER_ROW_START_LOCATION;
static uint16_t saucer_status = SAUCER_ALIVE;
static uint32_t saucer_shot_count = 0;
static uint16_t total_alien_count = STARTING_ALIEN_AMOUNT;

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

// fetches the current score of the current game
// returns : the score of the current game
uint32_t globals_get_current_score() {
  return current_score;
}

// adds a value to the current score
// score : the value of the score to add to the current score
void globals_add_to_current_score(uint32_t score) {
  current_score += score;
}

// fetches the position of the saucer
// returns the position of the saucer
uint32_t globals_get_saucer_pos() {
  return saucer_pos;
}

// sets the position of the saucer
// pos : the position which we wish to place the saucer
void globals_set_saucer_pos(uint32_t pos) {
  saucer_pos = pos;
}

// fetches the status of the saucer
// returns the status of the saucer
uint16_t globals_get_saucer_status() {
  return saucer_status;
}

// sets the status of the saucer
// status : the status that we wish to set for the saucer
void globals_set_saucer_status(uint16_t status) {
  saucer_status = status;
}

// fetches the saucer delay count
// returns : the delay count
uint32_t globals_get_saucer_shot_count() {
  return saucer_shot_count;
}

// increments the saucer shot count until it reaches a certain delay and resets
// the saucer start position
void globals_inc_saucer_shot_count() {
  saucer_shot_count++;
}

// resets the saucer shot count
void globals_reset_saucer_shot_count() {
  saucer_shot_count = 0;
}

// fetches the total alien count
// returns : the total alien count
uint16_t globals_get_total_alien_count() {
  return total_alien_count;
}

// decreases the total alien count by one when an alien gets shot
void globals_decrement_total_alien_count() {
  total_alien_count--;
}

// resets the total alien amount
void globals_reset_total_alien_count() {
  total_alien_count = STARTING_ALIEN_AMOUNT;
}
