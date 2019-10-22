#include "globals.h"
#include "../image_render/score_board/score_board.c"

/********************************** macros ***********************************/
#define STARTING_ALIEN_AMOUNT 55                                                // number of aliens at the start of the game
#define NOT_FIRED 0                                                             // indicates bullet not fired
#define FIRED 1                                                                 // indicates if bullet was fired
#define SAUCER_ALIVE 1                                                          // indicates if Saucer is alive
#define SAUCER_SHOT 0                                                           // indicates if saucer is dead
#define MAX_LIVES 5                                                             // indicates what the Max number of lives if
#define S_LOCATION_SC 60                                                        // indicates the location of the S character
#define C_LOCATION_SC S_LOCATION_SC+54                                          // indicates the location of the C character
#define O_LOCATION_SC C_LOCATION_SC+54                                          // indicates the location of the O character
#define R_LOCATION_SC O_LOCATION_SC+54                                          // indicates the locaiton of the R character
#define E_LOCATION_SC R_LOCATION_SC+54                                          // indicates the location of the E character
#define SCORE_0_LOCATION E_LOCATION_SC+108                                      // indicates the location of the score location
#define SCORE_1_LOCATION SCORE_0_LOCATION+54                                    // indicates the location of the score location
#define SCORE_2_LOCATION SCORE_1_LOCATION+54                                    // indicates the location of the score location
#define SCORE_3_LOCATION SCORE_2_LOCATION+54                                    // indicates the location of the score location
#define SCORE_4_LOCATION SCORE_3_LOCATION+54                                    // indicates the location of the score location
#define FIFTH_LIFE_LOCATION ((640*3)-(2*17*3)-9)                                // indicates the location of the tank life
#define FOURTH_LIFE_LOCATION (FIFTH_LIFE_LOCATION-(2*17*3)-9)                   // indicates the location of the tank life
#define THIRD_LIFE_LOCATION (FOURTH_LIFE_LOCATION-(2*17*3)-9)                   // indicates the location of the tank life
#define SECOND_LIFE_LOCATION (THIRD_LIFE_LOCATION-(2*17*3)-9)                   // indicates the location of the tank life
#define FIRST_LIFE_LOCATION (SECOND_LIFE_LOCATION-(2*17*3)-9)                   // indicates the location of the tank life
#define GLOBAL_BYTES_PER_PIXEL 3                                                // Global number of bytes per pixel
#define START_LIVES 3                                                           // number of lives you start with
#define TANK_SIZING 2                                                           // scaling for tank sprite
#define ONE_LIFE 1                                                              // indicates one life
#define TWO_LIFE 2                                                              // indicates that there are 2 lives
#define THREE_LIFE 3                                                            // indicates that there are 3 lives
#define FOUR_LIFE 4                                                             // indicates that there are 4 lives
#define FIVE_LIFE 5                                                             // indicates that there are 5 lives

/********************************** globals **********************************/
static uint16_t tank_bullet_fired = NOT_FIRED;                                  // flag for if tank bullet had been fired
static uint32_t tank_bullet_position;                                           // position of the tank bullet position
static uint16_t alien_bullet_fired_0 = NOT_FIRED;                               // flag for if alien bullet 0 has been shot
static uint16_t alien_bullet_fired_1 = NOT_FIRED;                               // flag for if alien bullet 1 has been shot
static uint16_t alien_bullet_fired_2 = NOT_FIRED;                               // flag for if alien bullet 2 has been shot
static uint16_t alien_bullet_fired_3 = NOT_FIRED;                               // flag for if alien bullet 3 has been shot
static uint32_t alien_bullet_position_0;                                        // Position of the alien bullet number 0
static uint32_t alien_bullet_position_1;                                        // Position of the alien bullet number 1
static uint32_t alien_bullet_position_2;                                        // Position of the alien bullet number 2
static uint32_t alien_bullet_position_3;                                        // Position of the alien bullet number 3
static uint32_t current_score = 0;                                              // The current score of the user, starts at zero
static uint32_t saucer_pos = GLOBALS_SAUCER_ROW_START_LOCATION;                 // Position of the saucer
static uint16_t saucer_status = SAUCER_ALIVE;                                   // flag for if saucer is alive
static uint32_t saucer_shot_count = 0;                                          // counter for how long the saucer has been shot for
static uint16_t total_alien_count = STARTING_ALIEN_AMOUNT;                      // tracks how many aliens are still alive
static uint32_t current_lives = START_LIVES;                                    // tracks the current number of lives
static uint16_t alien_overrun_flag = 0;                                         // tracks if the aliens have gon past the bunker line
static uint32_t dead_alien_loc = 0;                                             // tracks which aliens are dead

uint32_t global_green[GLOBAL_BYTES_PER_PIXEL] = {0x00,0x80,0x00};               // Global variable for the color green
uint32_t global_black[GLOBAL_BYTES_PER_PIXEL] = {0x00,0x00,0x00};               // Global variable for the color black

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

// set this to 0 once the bullet hits a target or reaches the top of the screen, allows the tank to fire another bullet
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

// fetch whether the alien bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_0(){
  return alien_bullet_fired_0;
}

// fetch whether the alien bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_1(){
  return alien_bullet_fired_1;
}

// fetch whether the alien bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_2(){
  return alien_bullet_fired_2;
}

// fetch whether the alien bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_3(){
  return alien_bullet_fired_3;
}

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the bottom of the screen
void globals_fire_alien_bullet_0(){
  alien_bullet_fired_0 = FIRED;
}

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the bottom of the screen
void globals_fire_alien_bullet_1(){
  alien_bullet_fired_1 = FIRED;
}

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the bottom of the screen
void globals_fire_alien_bullet_2(){
  alien_bullet_fired_2 = FIRED;
}

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the bottom of the screen
void globals_fire_alien_bullet_3(){
  alien_bullet_fired_3 = FIRED;
}

// set this to 0 once the bullet hits a target or reaches the bottom of the screen, allows the alien to fire another bullet
void globals_alien_bullet_stopped_0() {
  alien_bullet_fired_0 = NOT_FIRED;
}

// set this to 0 once the bullet hits a target or reaches the bottom of the screen, allows the alien to fire another bullet
void globals_alien_bullet_stopped_1() {
  alien_bullet_fired_1 = NOT_FIRED;
}

// set this to 0 once the bullet hits a target or reaches the bottom of the screen, allows the alien to fire another bullet
void globals_alien_bullet_stopped_2() {
  alien_bullet_fired_2 = NOT_FIRED;
}

// set this to 0 once the bullet hits a target or reaches the bottom of the screen, allows the alien to fire another bullet
void globals_alien_bullet_stopped_3() {
  alien_bullet_fired_3 = NOT_FIRED;
}

// fetch the current alien bullet position
// returns : the current alien bullet position
uint32_t globals_get_alien_bullet_position_0(){
  return alien_bullet_position_0;
}

// fetch the current alien bullet position
// returns : the current alien bullet position
uint32_t globals_get_alien_bullet_position_1(){
  return alien_bullet_position_1;
}

// fetch the current alien bullet position
// returns : the current alien bullet position
uint32_t globals_get_alien_bullet_position_2(){
  return alien_bullet_position_2;
}

// fetch the current alien bullet position
// returns : the current alien bullet position
uint32_t globals_get_alien_bullet_position_3(){
  return alien_bullet_position_3;
}

// set a new alien bullet position
// pos : the new position which you wish to set
void globals_set_alien_bullet_position_0(uint32_t pos){
  alien_bullet_position_0 = pos;
}

// set a new alien bullet position
// pos : the new position which you wish to set
void globals_set_alien_bullet_position_1(uint32_t pos){
  alien_bullet_position_1 = pos;
}

// set a new alien bullet position
// pos : the new position which you wish to set
void globals_set_alien_bullet_position_2(uint32_t pos){
  alien_bullet_position_2 = pos;
}

// set a new alien bullet position
// pos : the new position which you wish to set
void globals_set_alien_bullet_position_3(uint32_t pos){
  alien_bullet_position_3 = pos;
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

// prints current score
void globals_print_current_score(){
  uint32_t digit_1 = 0;                                                         // tracks the o 1's digit place of score
  uint32_t digit_10 = 0;                                                        // tracks the o 10's digit place of score
  uint32_t digit_100 = 0;                                                       // tracks the o 100's digit place of score
  uint32_t digit_1000 = 0;                                                      // tracks the o 1000's digit place of score
  uint32_t digit_10000 = 0;                                                     // tracks the o 10000's digit place of score
  uint32_t temp = globals_get_current_score();                                  // get the current score of the player
  digit_10000 = ((temp)/TEN_THOUSAND_SCALE);                                    // calculates the 10000 digit place of the score
  digit_1000 = ((temp-digit_10000*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);          // calculates the 1000 digit place of the score
  digit_100 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE)/HUNDRED_SCALE);// calculates the 100 digit place of the score
  digit_10 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE-digit_100*HUNDRED_SCALE)/TEN_SCALE);// calculates the 10 digit place of the score
  digit_1 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE-digit_100*HUNDRED_SCALE-digit_10*TEN_SCALE)/ONE_SCALE);// calculates the 1 digit place of the score
  //renders the 10000th digit place of score to screen
  sprites_render_buffer(char_array[digit_10000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_0_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  //renders the 1000th digit place of score to screen
  sprites_render_buffer(char_array[digit_1000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_1_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  //renders the 100th digit place of score to screen
  sprites_render_buffer(char_array[digit_100],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_2_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  //renders the 10th digit place of score to screen
  sprites_render_buffer(char_array[digit_10],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_3_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  //renders the 1th digit place of score to screen
  sprites_render_buffer(char_array[digit_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_4_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
}

// get the lives counter
// returns : the current lives of player
uint32_t globals_get_current_lives(){
  return current_lives;
}

// increments the current amount of lives upon defeating an entire alien block if you have less than five
void globals_increment_current_lives() {
  if(current_lives < MAX_LIVES)  {                                              // checks to see if we have reached the max amount of lives
    current_lives++;                                                            // increments current lives
  }
}

// decrements the current lives if the tank gets shot
void globals_decrement_current_lives() {
  if (current_lives > 0) {                                                      // checks to see if there are still lives
    current_lives--;                                                            // decrements current lives
  }
}

// fetches the flag that indicates that aliens have overrun our bunker
// returns the overrun flag
uint16_t globals_get_alien_overrun_flag() {
  return alien_overrun_flag;
}

// asserts the overrun flag if the bottom column of aliens reaches the top of the bunker line
void globals_assert_alien_overrun_flag() {
  alien_overrun_flag = 1;
}

// sets the location of the most recently killed alien
// loc : location of the most recently killed alien
void globals_set_dead_alien_loc(uint32_t loc) {
  dead_alien_loc = loc;
}

// fetches the location of the most recently killed alien
// returns the locaiton of the most recently killed alien
uint32_t globals_get_dead_alien_loc() {
  return dead_alien_loc;
}

// prints lives to screen
void globals_print_current_lives(){
  if(current_lives == ONE_LIFE){ // if we have one life left, delete four tanks and write one
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIRST_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,SECOND_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,THIRD_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FOURTH_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIFTH_LIFE_LOCATION,TANK_SIZING,global_black);
  }
  if(current_lives == TWO_LIFE){ // if we have two lives left, delete three tanks and write two
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIRST_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,SECOND_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,THIRD_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FOURTH_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIFTH_LIFE_LOCATION,TANK_SIZING,global_black);
  }
  if(current_lives == THREE_LIFE){ // if we have three lives left, delete two tanks and write three
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIRST_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,SECOND_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,THIRD_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FOURTH_LIFE_LOCATION,TANK_SIZING,global_black);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIFTH_LIFE_LOCATION,TANK_SIZING,global_black);
  }
  if(current_lives == FOUR_LIFE){ // if we have four lives left, delete one tanks and write four
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIRST_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,SECOND_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,THIRD_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FOURTH_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIFTH_LIFE_LOCATION,TANK_SIZING,global_black);
  }
  if(current_lives == FIVE_LIFE){ // if we have no lives left, write five tanks
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIRST_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,SECOND_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,THIRD_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FOURTH_LIFE_LOCATION,TANK_SIZING,global_green);
    sprites_render_buffer(tank_15x8,SPRITES_TANK_WIDTH-TANK_SIZING,SPRITES_TANK_HEIGHT-TANK_SIZING,FIFTH_LIFE_LOCATION,TANK_SIZING,global_green);
  }
}
