#include "globals.h"

/********************************** macros ***********************************/
#define RIGHT 0
#define LEFT 1
#define STARTING_LEFT_COLUMN 0
#define STARTING_RIGHT_COLUMN 10
#define STARTING_ALIEN_AMOUNT 55
#define ALIEN_ALIVE 1

// from main.c
#define BOUNCE 6000000 /* value of the ticks to seconds */
#define BTN_0_MASK 0x1 /* mask for button 0 */
#define BTN_1_MASK 0x2 /* mask for button 1 */
#define BTN_2_MASK 0x4 /* mask for button 2 */
#define BUFFER_SPACE 10
#define LETTER_A 11
#define CHAR_ARRAY_MAX 36
#define BYTES_PER_PIXEL 3
#define POSITION_ONE 0
#define POSITION_TWO 1
#define POSITION_THREE 2
#define MAX_COUNTER 3


/********************************** globals **********************************/
static uint16_t aliens_alive[STARTING_ALIEN_AMOUNT] = {ALIEN_ALIVE};
static uint16_t aliens_movement = RIGHT;
static uint16_t alien_block_far_left = STARTING_LEFT_COLUMN;
static uint16_t alien_block_far_right = STARTING_RIGHT_COLUMN;
