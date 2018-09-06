#include "globals.h"
#include <stdbool.h>
#include "../image_render/score_board/score_board.c"

/********************************** macros ***********************************/
#define STARTING_ALIEN_AMOUNT 55
#define NOT_FIRED 0
#define FIRED 1
#define SAUCER_ALIVE 1
#define SAUCER_SHOT 0
#define MAX_LIVES 5
#define S_LOCATION_SC 60
#define C_LOCATION_SC S_LOCATION_SC+54
#define O_LOCATION_SC C_LOCATION_SC+54
#define R_LOCATION_SC O_LOCATION_SC+54
#define E_LOCATION_SC R_LOCATION_SC+54
#define SCORE_0_LOCATION E_LOCATION_SC+108
#define SCORE_1_LOCATION SCORE_0_LOCATION+54
#define SCORE_2_LOCATION SCORE_1_LOCATION+54
#define SCORE_3_LOCATION SCORE_2_LOCATION+54
#define SCORE_4_LOCATION SCORE_3_LOCATION+54
#define FIFTH_LIFE_LOCATION ((640*3)-(2*17*3)-9)
#define FOURTH_LIFE_LOCATION (FIFTH_LIFE_LOCATION-(2*17*3)-9)
#define THIRD_LIFE_LOCATION (FOURTH_LIFE_LOCATION-(2*17*3)-9)
#define SECOND_LIFE_LOCATION (THIRD_LIFE_LOCATION-(2*17*3)-9)
#define FIRST_LIFE_LOCATION (SECOND_LIFE_LOCATION-(2*17*3)-9)
#define GLOBAL_BYTES_PER_PIXEL 3
#define START_LIVES 3
#define TANK_SIZING 2
#define ONE_LIFE 1
#define TWO_LIFE 2
#define THREE_LIFE 3
#define FOUR_LIFE 4
#define FIVE_LIFE 5

/********************************** globals **********************************/
static uint16_t tank_bullet_fired = NOT_FIRED;
static uint32_t tank_bullet_position;
static uint16_t alien_bullet_fired_0 = NOT_FIRED;
static uint16_t alien_bullet_fired_1 = NOT_FIRED;
static uint16_t alien_bullet_fired_2 = NOT_FIRED;
static uint16_t alien_bullet_fired_3 = NOT_FIRED;
static uint32_t alien_bullet_position_0;
static uint32_t alien_bullet_position_1;
static uint32_t alien_bullet_position_2;
static uint32_t alien_bullet_position_3;
static uint32_t current_score = 0;
static uint32_t saucer_pos = GLOBALS_SAUCER_ROW_START_LOCATION;
static uint16_t saucer_status = SAUCER_ALIVE;
static uint32_t saucer_shot_count = 0;
static uint16_t total_alien_count = STARTING_ALIEN_AMOUNT;
static uint32_t current_lives = START_LIVES;
static uint16_t alien_overrun_flag = 0;
static uint32_t dead_alien_loc = 0;
static bool tank_ex_flag = false;
static bool shoot_flag = false;
static bool alien_ex_flag = false;
static bool saucer_ex_flag = false;
static bool saucer_zoom_flag = false;
static bool alien_walk_flag = false;
uint32_t global_green[GLOBAL_BYTES_PER_PIXEL] = {0x00,0x80,0x00};
uint32_t global_black[GLOBAL_BYTES_PER_PIXEL] = {0x00,0x00,0x00};

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
  uint32_t digit_1 = 0;
  uint32_t digit_10 = 0;
  uint32_t digit_100 = 0;
  uint32_t digit_1000 = 0;
  uint32_t digit_10000 = 0;
  uint32_t temp = globals_get_current_score();
  digit_10000 = ((temp)/TEN_THOUSAND_SCALE);
  digit_1000 = ((temp-digit_10000*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);
  digit_100 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE)/HUNDRED_SCALE);
  digit_10 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE-digit_100*HUNDRED_SCALE)/TEN_SCALE);
  digit_1 = ((temp-digit_10000*TEN_THOUSAND_SCALE-digit_1000*THOUSAND_SCALE-digit_100*HUNDRED_SCALE-digit_10*TEN_SCALE)/ONE_SCALE);
  sprites_render_buffer(char_array[digit_10000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_0_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[digit_1000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_1_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[digit_100],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_2_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[digit_10],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_3_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[digit_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_4_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
}

// get the lives counter
uint32_t globals_get_current_lives(){
  return current_lives;
}

// increments the current amount of lives upon defeating an entire alien block if you have less than five
void globals_increment_current_lives() {
  if(current_lives < MAX_LIVES)  { // checks to see if we have reached the max amount of lives
    current_lives++;
  }
}

// decrements the current lives if the tank gets shot
void globals_decrement_current_lives() {
  if (current_lives > 0) {
    current_lives--;
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

// sets the tank explosion flag
// flag : wether the flag is high or low
void globals_set_tank_ex_flag(bool flag) {
  tank_ex_flag = flag;
}

// sets the shoot sound flag
// flag : wether the flag is high or low
void globals_set_shoot_flag(bool flag) {
  shoot_flag = flag;
}

// sets the alien explosion flag
// flag : wether the flag is high or low
void globals_set_alien_ex_flag(bool flag) {
  alien_ex_flag = flag;
}

// sets the saucer explosion flag
// flag : wether the flag is high or low
void globals_set_saucer_ex_flag(bool flag) {
  saucer_ex_flag = flag;
}

// sets the saucer move flag
// flag : wether the flag is high or low
void globals_set_saucer_zoom_flag(bool flag) {
  saucer_zoom_flag = flag;
}

// sets the aliens walking flag
// flag : wether the flag is high or low
void globals_set_alien_walk_flag(bool flag) {
  alien_walk_flag = flag;
}

// fetches the tank explosion flag
// returns : tank explosion flag
bool globals_get_tank_ex_flag() {
  return tank_ex_flag;
}

// fetches the shoot flag
// returns : the shoot flag
bool globals_get_shoot_flag() {
  return shoot_flag;
}

// fetches the alien explosion flag
// returns : alien explosion flag
bool globals_get_alien_ex_flag() {
  return alien_ex_flag;
}

// fetches the saucer explosion flag
// returns : saucer explosion flag
bool globals_get_saucer_ex_flag() {
  return saucer_ex_flag;
}

// fetches the saucer zoom flag
// returns : saucer zoom flag
bool globals_get_saucer_zoom_flag() {
  return saucer_zoom_flag;
}

// fetches the alien walk flag
// returns : alien walk flag
bool globals_get_alien_walk_flag() {
  return alien_walk_flag;
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
