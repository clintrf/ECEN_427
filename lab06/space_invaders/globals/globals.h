#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
/********************************** macros ***********************************/
#define GLOBALS_SAUCER_ROW_START_LOCATION (15+(640*3)*20)

/********************************* functions *********************************/

// fetch whether the tank bullet has been fired or not
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_tank_bullet_fired();

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the top of the screen
void globals_fire_tank_bullet();

// set this to 0 once the bullet hits a target or reaches the top of the screen, allows the tank to fire another bullet
void globals_tank_bullet_stopped();

// fetch the current tank bullet position
// returns : the current tank position
uint32_t globals_get_tank_bullet_position();

// set a new tank bullet position
// pos : the new position which you wish to set
void globals_set_tank_bullet_position(uint32_t pos);

// fetch whether the alien bullet has been fired or not
// we have a function for each individual alien bullet
// returns : a 1 if the bullet is still on the screen or a 0 if there is no bullet on screen
uint16_t globals_get_alien_bullet_fired_0();
uint16_t globals_get_alien_bullet_fired_1();
uint16_t globals_get_alien_bullet_fired_2();
uint16_t globals_get_alien_bullet_fired_3();

// set this to 1 if a bullet has been fired, keep it asserted until the bullet hits a target or reaches the top of the screen
// we have a function for each individual alien bullet
void globals_fire_alien_bullet_0();
void globals_fire_alien_bullet_1();
void globals_fire_alien_bullet_2();
void globals_fire_alien_bullet_3();

// set this to 0 once the bullet hits a target or reaches the bottom of the screen, allows the alien to fire another bullet
// we have a function for each individual alien bullet
void globals_alien_bullet_stopped_0();
void globals_alien_bullet_stopped_1();
void globals_alien_bullet_stopped_2();
void globals_alien_bullet_stopped_3();

// fetch the current alien bullet position
// returns : the current alien bullet position
// we have a function for each individual alien bullet
uint32_t globals_get_alien_bullet_position_0();
uint32_t globals_get_alien_bullet_position_1();
uint32_t globals_get_alien_bullet_position_2();
uint32_t globals_get_alien_bullet_position_3();

// set a new alien bullet position
// pos : the new position which you wish to set
// we have a function for each individual alien bullet
void globals_set_alien_bullet_position_0(uint32_t pos);
void globals_set_alien_bullet_position_1(uint32_t pos);
void globals_set_alien_bullet_position_2(uint32_t pos);
void globals_set_alien_bullet_position_3(uint32_t pos);

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

// sets the location of the most recently killed alien
// loc : location of the most recently killed alien
void globals_set_dead_alien_loc(uint32_t loc);

// fetches the location of the most recently killed alien
// returns the locaiton of the most recently killed alien
uint32_t globals_get_dead_alien_loc();

/***************************** GLOBALS FOR AUDIO *****************************/
// sets the tank explosion flag
// flag : wether the flag is high or low
void globals_set_tank_ex_flag(bool flag);

// sets the shoot sound flag
// flag : wether the flag is high or low
void globals_set_shoot_flag(bool flag);

// sets the alien explosion flag
// flag : wether the flag is high or low
void globals_set_alien_ex_flag(bool flag);

// sets the saucer explosion flag
// flag : wether the flag is high or low
void globals_set_saucer_ex_flag(bool flag);

// sets the saucer move flag
// flag : wether the flag is high or low
void globals_set_saucer_zoom_flag(bool flag);

// sets the aliens walking flag
// flag : wether the flag is high or low
void globals_set_alien_walk_flag(bool flag);

// fetches the tank explosion flag
// returns : tank explosion flag
bool globals_get_tank_ex_flag();

// fetches the shoot flag
// returns : the shoot flag
bool globals_get_shoot_flag();

// fetches the alien explosion flag
// returns : alien explosion flag
bool globals_get_alien_ex_flag();

// fetches the saucer explosion flag
// returns : saucer explosion flag
bool globals_get_saucer_ex_flag();

// fetches the saucer zoom flag
// returns : saucer zoom flag
bool globals_get_saucer_zoom_flag();

// fetches the alien walk flag
// returns : alien walk flag
bool globals_get_alien_walk_flag();

// determines whether the tank is alive or not
// flag : whether the tank is dead or not
void globals_set_tank_dead(bool flag);

// fetches whether the tank is dead or not
// flag : tank_dead_flag
bool globals_get_tank_dead();
