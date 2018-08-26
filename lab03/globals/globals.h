#include <stdio.h>
#include <stdint.h>

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

// fetches the current score of the current game
// returns : the score of the current game
uint32_t globals_get_current_score();

// adds a value to the current score
// score : the value of the score to add to the current score
void globals_add_to_current_score(uint32_t score);
