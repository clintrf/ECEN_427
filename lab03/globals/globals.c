#include "globals.h"
#include "../image_render/score_board/score_board.c"

/********************************** macros ***********************************/
#define STARTING_LEFT_COLUMN 0
#define STARTING_RIGHT_COLUMN 10
#define STARTING_ALIEN_AMOUNT 55
#define ALIEN_ALIVE 1
#define NOT_FIRED 0
#define FIRED 1
#define SAUCER_ALIVE 1
#define SAUCER_SHOT 0

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

/********************************** globals **********************************/
static uint16_t tank_bullet_fired = NOT_FIRED;
static uint32_t tank_bullet_position;
static uint32_t current_score = 0;
static uint32_t saucer_pos = GLOBALS_SAUCER_ROW_START_LOCATION;
static uint16_t saucer_status = SAUCER_ALIVE;
static uint32_t saucer_shot_count = 0;
static uint16_t total_alien_count = STARTING_ALIEN_AMOUNT;
static uint32_t current_lives = START_LIVES;

uint32_t global_green[GLOBAL_BYTES_PER_PIXEL] = {0x00,0x80,0x00};

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


void globals_print_current_score(){
  uint32_t diget_1 = 0;
  uint32_t diget_10 = 0;
  uint32_t diget_100 = 0;
  uint32_t diget_1000 = 0;
  uint32_t diget_10000 = 0;

  uint32_t temp = globals_get_current_score();

  diget_10000 = ((temp)/TEN_THOUSAND_SCALE);
  diget_1000 = ((temp-diget_10000*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);
  diget_100 = ((temp-diget_10000*TEN_THOUSAND_SCALE-diget_1000*THOUSAND_SCALE)/HUNDRED_SCALE);
  diget_10 = ((temp-diget_10000*TEN_THOUSAND_SCALE-diget_1000*THOUSAND_SCALE-diget_100*HUNDRED_SCALE)/TEN_SCALE);
  diget_1 = ((temp-diget_10000*TEN_THOUSAND_SCALE-diget_1000*THOUSAND_SCALE-diget_100*HUNDRED_SCALE-diget_10*TEN_SCALE)/ONE_SCALE);


  sprites_render_buffer(char_array[diget_10000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_0_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[diget_1000],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_1_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[diget_100],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_2_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[diget_10],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_3_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
  sprites_render_buffer(char_array[diget_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,SCORE_4_LOCATION,SPRITES_NORMAL_CHARACTER_SCALING,global_green);
}

uint32_t globals_get_current_lives(){
  current_lives;
}

void globals_print_current_lives(){
  if(current_lives>=1){
    sprites_render_buffer(tank_15x8,15,8,FIRST_LIFE_LOCATION,2,global_green);
  }
  if(current_lives>=2){
    sprites_render_buffer(tank_15x8,15,8,SECOND_LIFE_LOCATION,2,global_green);
  }
  if(current_lives>=3){
    sprites_render_buffer(tank_15x8,15,8,THIRD_LIFE_LOCATION,2,global_green);
  }
  if(current_lives>=4){
    sprites_render_buffer(tank_15x8,15,8,FOURTH_LIFE_LOCATION,2,global_green);
  }
  if(current_lives>=5){
    sprites_render_buffer(tank_15x8,15,8,FIFTH_LIFE_LOCATION,2,global_green);
  }

}
