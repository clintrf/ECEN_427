#include "globals.h"

/********************************** macros ***********************************/
#define RIGHT 0
#define LEFT 1
#define STARTING_LEFT_COLUMN 0
#define STARTING_RIGHT_COLUMN 10
#define STARTING_ALIEN_AMOUNT 55
#define ALIEN_ALIVE 1

/********************************** globals **********************************/
static uint16_t aliens_alive[STARTING_ALIEN_AMOUNT] = {ALIEN_ALIVE};
static uint16_t aliens_movement = RIGHT;
static uint16_t alien_block_far_left = STARTING_LEFT_COLUMN;
static uint16_t alien_block_far_right = STARTING_RIGHT_COLUMN;
