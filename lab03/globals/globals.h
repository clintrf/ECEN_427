#include <stdio.h>
#include <stdint.h>

/********************************** macros ***********************************/
#define GLOBALS_SAUCER_ROW_START_LOCATION (15+(640*3)*20)

/********************************* functions *********************************/

// fetch whether the tank bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_tank_bullet_fired();

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the top of the screen
void globals_fire_tank_bullet();

// set this to 0 once the bullet hits a target or reaches the top of the screen, prevents the tank from firing more bullets
void globals_tank_bullet_stopped();

// fetch the current tank bullet position
// returns : the current tank position
uint32_t globals_get_tank_bullet_position();

// set a new tank bullet position
// pos : the new position which you wish to set
void globals_set_tank_bullet_position(uint32_t pos);

// fetch whether the tank bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_0();
uint16_t globals_get_alien_bullet_fired_1();
uint16_t globals_get_alien_bullet_fired_2();

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the top of the screen
void globals_fire_alien_bullet_0();
void globals_fire_alien_bullet_1();
void globals_fire_alien_bullet_2();

// set this to 0 once the bullet hits a target or reaches the top of the screen, prevents the alien from firing more bullets
void globals_alien_bullet_stopped_0();
void globals_alien_bullet_stopped_1();
void globals_alien_bullet_stopped_2();

// fetch the current alien bullet position
// returns : the current tank position
uint32_t globals_get_alien_bullet_position_0();
uint32_t globals_get_alien_bullet_position_1();
uint32_t globals_get_alien_bullet_position_2();

// set a new alien bullet position
// pos : the new position which you wish to set
void globals_set_alien_bullet_position_0(uint32_t pos);
void globals_set_alien_bullet_position_1(uint32_t pos);
void globals_set_alien_bullet_position_2(uint32_t pos);


// fetches the current score of the current game
// returns : the score of the current game
uint32_t globals_get_current_score();

// adds a value to the current score
// score : the value of the score to add to the current score
void globals_add_to_current_score(uint32_t score);

// fetches the position of the saucer
// returns the position of the saucer
uint32_t globals_get_saucer_pos();

// sets the position of the saucer
// pos : the position which we wish to place the saucer
void globals_set_saucer_pos(uint32_t pos);

// fetches the status of the saucer
// returns the status of the saucer
uint16_t globals_get_saucer_status();

// sets the status of the saucer
// status : the status that we wish to set for the saucer
void globals_set_saucer_status(uint16_t status);

// fetches the saucer delay count
// returns : the delay count
uint32_t globals_get_saucer_shot_count();

// increments the saucer shot count until it reaches a certain delay and resets
// the saucer start position
void globals_inc_saucer_shot_count();

// resets the saucer shot count
void globals_reset_saucer_shot_count();

// fetches the total alien count
// returns : the total alien count
uint16_t globals_get_total_alien_count();

// decreases the total alien count by one when an alien gets shot
void globals_decrement_total_alien_count();

// resets the total alien amount
void globals_reset_total_alien_count();

// prints current score
void globals_print_current_score();

// get the lives counter
uint32_t globals_get_current_lives();

// increments the current amount of lives upon defeating an entire alien block if you have less than five
void globals_increment_current_lives();

// decrements the current lives if the tank gets shot
void globals_decrement_current_lives();

// prints lives to screen
void globals_print_current_lives();

// fetches the flag that indicates that aliens have overrun our bunker
// returns the overrun flag
uint16_t globals_get_alien_overrun_flag();

// asserts the overrun flag if the bottom column of aliens reaches the top of the bunker line
void globals_assert_alien_overrun_flag();
