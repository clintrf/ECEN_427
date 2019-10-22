#include <stdint.h>
#include <stdio.h>

/********************************** macros ***********************************/
#define SPRITES_SAUCER_WIDTH 18																									// define the saucer width
#define SPRITES_SAUCER_HEIGHT 9																								  // define the saucer height
#define SPRITES_ALIEN_EXPLOSION_WIDTH 12																				// define the explosion width
#define SPRITES_ALIEN_EXPLOSION_HEIGHT 10																				// define the explosion height
#define SPRITES_ALIEN_WIDTH 14																									// define the alien width
#define SPRITES_ALIEN_HEIGHT 10																									// define the alien height
#define SPRITES_TANK_WIDTH 17																										// define the tank width
#define SPRITES_TANK_HEIGHT 10																									// define the tank height
#define SPRITES_BUNKER_WIDTH 24																									// define the bunker width
#define SPRITES_BUNKER_HEIGHT 18																								// define the bunker height
#define SPRITES_BUNKER_DAMAGE_WIDTH 6																						// define the bunker damage height
#define SPRITES_BUNKER_DAMAGE_HEIGHT 6																					// define the bunker damage width
#define SPRITES_TANK_BULLET_WIDTH 1																							// define the tank bullet width
#define SPRITES_ALIEN_BULLET_WIDTH 3																						// define the alien bullet width
#define SPRITES_BULLET_HEIGHT 7																									// define the bullet height
#define SPRITES_CHARACTER_WIDTH 5																								// define the general sprites width
#define SPRITES_CHARACTER_HEIGHT 5																							// define the general sprites height
#define SPRITES_SCALE_NEXT_LINE 640*3																						// define the number of pixels to to get to the next line
#define SPRITES_GAME_OVER_SCALING 8																							// define the scaler for the game over letters
#define SPRITES_NORMAL_CHARACTER_SCALING 3																			// define the normal scale value
#define MIDPOINT_WIDTH 960																											// define the mid point on the screen
#define CHARACTER_SPACE_GO 24																										// define the spaceing between letters
#define SPRITES_G_START_LOCATION_GO 360																					// define the start location of G
#define SPRITES_A_START_LOCATION_GO 504																					// define the start location of A
#define SPRITES_M_START_LOCATION_GO 648																					// define the start location of M
#define SPRITES_E_START_LOCATION_GO 792																					// define the start location of E
#define SPRITES_O_START_LOCATION_GO 1008																			  // define the start location of O
#define SPRITES_V_START_LOCATION_GO 1152																				// define the start location of V
#define SPRITES_E2_START_LOCATION_GO 1296																				// define the start location of E
#define SPRITES_R_START_LOCATION_GO 1440																				// define the start location of R
#define GAME_OVER_CHARACTER_HEIGHT SPRITES_GAME_OVER_SCALING*SPRITES_CHARACTER_HEIGHT // define the heigth of game over letters
#define GAME_OVER_END_LINE GAME_OVER_CHARACTER_HEIGHT*SPRITES_SCALE_NEXT_LINE   // define the Game over end line
#define EXTRA_INFORMATION_START_LINE GAME_OVER_END_LINE+(SPRITES_SCALE_NEXT_LINE*10)// define the position of the extra ubfirnatuib on the end screen
#define SPRITES_E_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-60-(45*7)	// define the start location of the E letter
#define SPRITES_N_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-54-(45*6)	// define the start location of the N letter
#define SPRITES_T_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-48-(45*5)	// define the start location of the T letter
#define SPRITES_E2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-42-(45*4)	// define the start location of the E letter
#define SPRITES_R_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-36-(45*3)	// define the start location of the R letter
#define SPRITES_Y_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-12-(45*2)	// define the start location of the Y letter
#define SPRITES_O_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-6-(45*1)	// define the start location of the O letter
#define SPRITES_U_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+6+(45*0)	// define the start location of the U letter
#define SPRITES_R2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+12+(45*1)	// define the start location of the R letter
#define SPRITES_N2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+36+(45*2)	// define the start location of the N letter
#define SPRITES_A_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+42+(45*3)	// define the start location of the A letter
#define SPRITES_M_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+48+(45*4)	// define the start location of the M letter
#define SPRITES_E3_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+54+(45*5)	// define the start location of the E letter
#define SPRITES_H_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-42-(45*4)	// define the start location of the H letter
#define SPRITES_I_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-36-(45*3)	// define the start location of the I letter
#define SPRITES_G_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-30-(45*2)	// define the start location of the G letter
#define SPRITES_H2_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-24-(45*1)	// define the start location of the H letter
#define SPRITES_S_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+(45*0)	// define the start location of the S letter
#define SPRITES_C_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+6+(45*1)	// define the start location of the C letter
#define SPRITES_O_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+12+(45*2)	// define the start location of the O letter
#define SPRITES_R_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+18+(45*3)	// define the start location of the R letter
#define SPRITES_E_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+24+(45*4)	// define the start location of the E letter
#define SPRITES_S2_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+30+(45*5)	// define the start location of the S letter
#define ENTER_NAME_CHARACTER_HEIGHT EXTRA_INFORMATION_START_LINE+NORMAL_CHARACTER_SCALING*SPRITES_CHARACTER_HEIGHT*SPRITES_SCALE_NEXT_LINE // position of where to enter the name
#define SPRITES_CHARACTER_ONE_START_LOCATION (SPRITES_SCORE_BOARD_START_POS+(0*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*10)) // postion of where the first letter of the player name goes
#define SPRITES_CHARACTER_TWO_START_LOCATION (SPRITES_SCORE_BOARD_START_POS+(1*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*10)) // postion of where the second letter of the player name goes
#define SPRITES_CHARACTER_THREE_START_LOCATION (SPRITES_SCORE_BOARD_START_POS+(2*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*10)) // postion of where the third letter of the player name goes

#define PIXEL_SPACING 3																													// spaceing between pixels
#define SPRITES_ONE_PIXEL 3																											// bytes per pixel for the sprites
#define SPRITES_SCORE_BOARD_START_POS 640*100*3+(150*3)													// postion for where to start the score board

#define WIDTH_PIXAL_COUNT_UNIT 42																								// Width of pixels of default sprites

static const uint32_t saucer_16x7[] =
{																																								// image of Saucer
	0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0
};

static const uint32_t alien_explosion_12x10[] =
{																																								// image of alien explotion
	0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,1,0,0,1,0,0,0,1,0,
	1,0,0,1,0,0,0,0,0,1,0,0,
	0,1,0,0,1,0,0,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,
	1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,1,0,0,1,0,
	0,0,1,0,0,0,0,0,1,0,0,1,
	0,1,0,0,0,1,0,0,1,0,0,0,
	0,0,0,0,0,1,0,0,0,0,0,0
};

static const uint32_t alien_top_in_12x8[] =
{																																								// image of alien on top row moving in
	0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,0,1,1,0,1,0,0,0,
	0,0,1,0,0,0,0,0,0,1,0,0,
	0,0,0,1,0,0,0,0,1,0,0,0
};

static const uint32_t alien_top_out_12x8[] =
{																																								// image of alien on top row moving out
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,1,0,1,1,0,1,0,0,0,
	0,0,1,0,1,0,0,1,0,1,0,0
};

static const uint32_t alien_middle_in_12x8[] =
{																																								// image of alien in middle row moving in
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,0,1,0,0,0,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,1,1,1,0,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,1,0,1,0,0,0,0,0,1,0,1,
	0,0,0,0,1,1,0,1,1,0,0,0
};

static const uint32_t alien_middle_out_12x8[] =
{																																								// image of alien in middle row moving out
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,1,0,0,1,0,0,0,1,0,0,1,
	0,1,0,1,1,1,1,1,1,1,0,1,
	0,1,1,1,0,1,1,1,0,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,0,0,0,1,0
};

static const uint32_t alien_bottom_in_12x8[] =
{																																								// image of alien in bottom row moving in
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,0,0,1,1,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,1,1,0,0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,0,0,1,1,0,0
};

static const uint32_t alien_bottom_out_12x8[] =
{																																								// image of alien in bottom row moving out
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,0,0,1,1,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,1,0,0,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	1,1,0,0,0,0,0,0,0,0,1,1
};

static const uint32_t tank_15x8[] =
{																																								// image of Tank
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};


// Shape of the entire bunker.
static const uint32_t bunker_24x18[] =
{																																								// image of bunker
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const uint32_t bunkerDamage0_6x6[] = {																		// damage on bunker, discription in name
	0,1,1,0,0,0,
	0,0,0,0,0,1,
	1,1,0,1,0,0,
	1,0,0,0,0,0,
	0,0,1,1,0,0,
	0,0,0,0,1,0
};

static const uint32_t bunkerDamage1_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,0,1,0,
	1,0,1,0,0,1,
	1,1,0,1,1,1,
	1,0,0,0,0,0,
	0,1,1,1,0,1,
	0,1,1,0,1,0
};

static const uint32_t bunkerDamage2_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,0,1,1,0,1,
	1,1,0,1,1,1,
	1,1,0,1,1,0,
	0,1,1,1,0,1,
	1,1,1,1,1,1
};

static const uint32_t bunkerDamage3_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_upper_left_gone_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,1,1,1,
	0,0,1,1,1,1,
	0,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_upper_left_damage2_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,0,1,1,
	0,0,1,0,1,1,
	0,1,1,1,1,0,
	1,0,1,0,1,1,
	1,0,1,1,1,1,
	1,1,1,0,1,1
};

static const uint32_t bunker_upper_left_damage1_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,0,0,0,
	0,0,0,0,1,1,
	0,1,1,0,1,0,
	0,0,1,0,1,1,
	0,0,1,1,1,1,
	1,1,1,0,1,1
};

static const uint32_t bunker_upper_left_damage0_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,0,0,0,
	0,0,0,0,0,0,
	0,0,0,0,1,0,
	0,0,0,0,1,1,
	0,0,1,1,1,1,
	0,0,1,0,1,1
};

static const uint32_t bunker_upper_right_gone_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,0,0,0,
	1,1,1,1,0,0,
	1,1,1,1,1,0,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_upper_right_damage2_6x6[] = {																		// damage on bunker, discription in name
	1,1,0,0,0,0,
	1,1,0,1,0,0,
	0,1,1,1,1,0,
	1,1,1,1,0,1,
	1,1,1,1,0,1,
	1,1,0,1,1,1
};

static const uint32_t bunker_upper_right_damage1_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,0,0,0,
	1,1,0,0,0,0,
	0,1,0,1,1,0,
	1,1,0,1,0,0,
	1,1,1,1,0,0,
	1,1,0,1,1,1
};

static const uint32_t bunker_upper_right_damage0_6x6[] = {																		// damage on bunker, discription in name
	0,0,0,0,0,0,
	0,0,0,0,0,0,
	0,1,0,0,0,0,
	1,1,0,0,0,0,
	1,1,1,1,0,0,
	1,1,0,1,0,0
};

static const uint32_t bunker_lower_middle_right_damage0_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,0,1,0,1,1,
	0,0,0,0,0,1,
	0,0,0,0,1,1,
	0,0,0,0,0,1,
	0,0,0,0,0,0
};

static const uint32_t bunker_lower_middle_right_gone_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	0,0,1,1,1,1,
	0,0,0,1,1,1,
	0,0,0,0,1,1,
	0,0,0,0,0,1
};

static const uint32_t bunker_lower_middle_left_damage0_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,1,0,1,0,1,
	1,0,0,0,0,0,
	1,1,0,0,0,0,
	1,0,0,0,0,0,
	0,0,0,0,0,0
};

static const uint32_t bunker_lower_middle_left_gone_6x6[] = {																		// damage on bunker, discription in name
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,0,0,
	1,1,1,0,0,0,
	1,1,0,0,0,0,
	1,0,0,0,0,0
};

static const uint32_t tank_explosion1_15x8[] = {																// image of tank explosion
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,
	1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
};

static const uint32_t tank_explosion2_15x8[] = {																// image of tank explosion
	1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,
	1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,0,0,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
};

static const uint32_t tank_gone_15x8[] = {																// image of tank gone
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tankbullet_1x5[] = {																// image of tank bullet
	1,
	1,
	1,
	1,
	1
};

static const uint32_t tankbullet_gone_1x5[] = {																// image of tank bullet gone
	0,
	0,
	0,
	0,
	0
};

static const uint32_t alienbullet1_down_3x5[] = {																// image of alien 1 bullet down
	0,1,0,
	0,1,0,
	0,1,0,
	1,1,1,
	0,1,0
};

static const uint32_t alienbullet1_up_3x5[] = {																// image of alien 1 bullet up
	0,1,0,
	1,1,1,
	0,1,0,
	0,1,0,
	0,1,0
};

static const uint32_t alienbullet2_up_3x5[] = {																// image of alien 2 bullet up
	0,1,0,
	1,0,0,
	0,1,0,
	0,0,1,
	0,1,0
};

static const uint32_t alienbullet2_down_3x5[] = {																// image of alien 2 bullet down
	0,1,0,
	0,0,1,
	0,1,0,
	1,0,0,
	0,1,0
};


static const uint32_t alienbullet2_gone_3x5[] = {																// image of alien 2 bullet gone
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
};

static const uint32_t letterA_5x5[] = {																          // image of letter
	0,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterB_5x5[] = {																          // image of letter
	1,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterC_5x5[] = {																          // image of letter
	0,1,1,1,1,
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	0,1,1,1,1
};

static const uint32_t letterD_5x5[] = {																          // image of letter
	1,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterE_5x5[] = {																          // image of letter
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,0,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t letterF_5x5[] = {																          // image of letter
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,0,
	1,0,0,0,0,
	1,0,0,0,0
};

static const uint32_t letterG_5x5[] = {																          // image of letter
	0,1,1,1,1,
	1,0,0,0,0,
	1,0,0,1,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterH_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterI_5x5[] = {																          // image of letter
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterJ_5x5[] = {																          // image of letter
	1,1,1,1,1,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	1,1,1,0,0
};

static const uint32_t letterK_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,0,0,1,0,
	1,1,1,0,0,
	1,0,0,1,0,
	1,0,0,0,1
};

static const uint32_t letterL_5x5[] = {																          // image of letter
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t letterM_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,1,0,1,1,
	1,0,1,0,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterN_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,1,0,0,1,
	1,0,1,0,1,
	1,0,0,1,1,
	1,0,0,0,1
};

static const uint32_t letterO_5x5[] = {																          // image of letter
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterP_5x5[] = {																          // image of letter
	0,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,0,
	1,0,0,0,0
};

static const uint32_t letterQ_5x5[] = {																          // image of letter
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,1,0,
	0,1,1,0,1
};

static const uint32_t letterR_5x5[] = {																          // image of letter
	1,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterS_5x5[] = {																          // image of letter
	0,1,1,1,1,
	1,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterT_5x5[] = {																          // image of letter
	1,1,1,1,1,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterU_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterV_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0
};

static const uint32_t letterW_5x5[] = {																          // image of letter
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,1,0,1,
	1,0,1,0,1,
	0,1,0,1,0
};

static const uint32_t letterX_5x5[] = {																          // image of letter
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0,
	0,1,0,1,0,
	1,0,0,0,1
};

static const uint32_t letterY_5x5[] = {																          // image of letter
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterZ_5x5[] = {																          // image of letter
	1,1,1,1,1,
	0,0,0,1,0,
	0,0,1,0,0,
	0,1,0,0,0,
	1,1,1,1,1
};

static const uint32_t number0_5x5[] = {																          // image of number discription in name
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t number1_5x5[] = {																          // image of number discription in name
	1,1,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	1,1,1,1,1
};

static const uint32_t number2_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t number3_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number4_5x5[] = {																          // image of number discription in name
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	0,0,0,0,1
};

static const uint32_t number5_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number6_5x5[] = {																          // image of number discription in name
	0,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number7_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	0,0,0,0,1,
	0,0,0,0,1,
	0,0,0,0,1,
	0,0,0,0,1
};

static const uint32_t number8_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number9_5x5[] = {																          // image of number discription in name
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t letterBLANK_5x5[] = {																          // image of letter discription in name
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0
};

static const uint32_t char_array[37][25] = {																		// array that hold mapping of characters
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, 												// 0			0
  {	1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1}, 												// 1			1
  {	1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1}, 												// 2			2
  {	1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, 												// 3			3
  {	1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1}, 												// 4			4
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, 												// 5			5
  {	0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1}, 												// 6			6
  {	1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, 												// 7			7
  {	1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1}, 												// 8			8
  {	1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, 												// 9			9
  {	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 												// BLANK	10
  {	0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1}, 												// A			11
  {	1,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0}, 												// B			12
  {	0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1}, 												// C			13
  {	1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0}, 												// D			14
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,1,1,1,1}, 												// E			15
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0}, 												// F			16
  {	0,1,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0}, 												// G			17
  {	1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1}, 												// H			18
  {	0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, 												// I			19
  {	1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0}, 												// J			20
  {	1,0,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,1}, 												// K			21
  {	1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1}, 												// L			22
  {	1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1}, 												// M			23
  {	1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1}, 												// N			24
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, 												// O			25
  {	0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0}, 												// P			26
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,1}, 												// Q			27
  {	1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1}, 												// R			28
  {	0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0}, 												// S			29
  {	1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, 												// T			30
  {	1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, 												// U			31
  {	1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0}, 												// V			32
  {	1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0}, 												// W			33
  {	1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1}, 												// X			34
  {	1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, 												// Y			35
  {	1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1}  												// Z			36
};

static const uint32_t letter_array[37] = {																			// maping for letters
  '0', 																																					// 0			0
  '1', 																																					// 1			1
  '2', 																																					// 2			2
  '3', 																																					// 3			3
  '4', 																																					// 4			4
  '5', 																																					// 5			5
  '6', 																																					// 6			6
  '7', 																																					// 7			7
  '8', 																																					// 8			8
  '9', 																																					// 9			9
  ' ', 																																					// BLANK	10
  'A', 																																					// A			11
  'B', 																																					// B			12
  'C',																																					// C			13
  'D',																																					// D			14
  'E', 																																					// E			15
  'F', 																																					// F			16
  'G', 																																					// G			17
  'H', 																																					// H			18
  'I', 																																					// I			19
  'J', 																																					// J			20
  'K', 																																					// K			21
  'L', 																																					// L			22
  'M', 																																					// M			23
  'N', 																																					// N			24
  'O', 																																					// O			25
  'P', 																																					// P			26
  'Q', 																																					// Q			27
  'R', 																																					// R			28
  'S', 																																					// S			29
  'T', 																																					// T			30
  'U', 																																					// U			31
  'V', 																																					// V			32
  'W', 																																					// W			33
  'X', 																																					// X			34
  'Y', 																																					// Y			35
  'Z'  																																					// Z			36
};

static const uint32_t saucer_18x9[] =
{																																								// image of buffered saucer
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t alien_explosion_14x12[] =
{																																								// image of buffered explosion
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,0,0,1,0,0,0,1,0,0,
	0,1,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,1,0,0,1,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,1,1,0,
	0,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,1,0,0,1,0,0,
	0,0,0,1,0,0,0,0,0,1,0,0,1,0,
	0,0,1,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t alien_top_in_14x10[] =
{																																								// image of buffered alien top row in movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
  0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,0,1,1,0,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,1,0,1,1,0,1,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,1,0,0,0,
	0,0,0,0,1,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static const uint32_t alien_top_out_14x10[] =
{																																								// image of buffered alien top row out movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,0,0,0,0,
	0,0,0,1,1,0,1,1,0,1,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,0,0,1,0,0,0,0,0,
	0,0,0,0,1,0,1,1,0,1,0,0,0,0,
	0,0,0,1,0,1,0,0,1,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static const uint32_t alien_middle_in_14x10[] =
{																																								// image of buffered alien middle row in movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,0,0,0,1,0,0,0,1,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,0,1,1,0,1,1,1,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,1,0,1,0,0,0,0,0,1,0,1,0,
	0,0,0,0,0,1,1,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t alien_middle_out_14x10[] =
{																																								// image of buffered alien middle row out movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,1,0,0,1,0,0,0,1,0,0,1,0,
	0,0,1,0,1,1,1,1,1,1,1,0,1,0,
	0,0,1,1,1,0,1,1,1,0,1,1,1,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t alien_bottom_in_14x10[] =
{																																								// image of buffered alien bottom row in movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,0,0,1,1,0,0,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,1,1,0,0,1,1,1,0,0,0,
	0,0,1,1,0,0,1,1,0,0,1,1,0,0,
	0,0,0,1,1,0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t alien_bottom_out_14x10[] =
{																																								// image of beffered alien mottom row out movement
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,0,0,1,1,0,0,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,1,1,0,0,1,1,0,0,0,0,
	0,0,0,1,1,0,1,1,0,1,1,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tank_17x10[] =
{																																								// image of buffered tank
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tank_explosion1_17x10[] =
{																																								// image of buffered tank explosion
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,
	0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,
	0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,
	0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tank_explosion2_17x10[] =
{																																								// image of buffered tank explosion
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,
	0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tank_gone_17x10[] =
{																																								// image of buffered tank gone
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tankbullet_1x7[] =
{																																								// image of buffered tank bullet gone
	0,
	1,
	1,
	1,
	1,
	1,
	0
};

static const uint32_t tankbullet_gone_1x7[] =
{																																								// image of buffered tank bullet gone
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static const uint32_t alienbullet1_down_3x7[] =
{																																								// image of buffered alien bullet
	0,0,0,
	0,1,0,
	0,1,0,
	0,1,0,
	1,1,1,
	0,1,0,
	0,0,0
};

static const uint32_t alienbullet1_up_3x7[] =
{																																								// image of buffered alien bullet
	0,0,0,
	0,1,0,
	1,1,1,
	0,1,0,
	0,1,0,
	0,1,0,
	0,0,0
};

static const uint32_t alienbullet2_up_3x7[] =
{																																								// image of buffered alien bullet
	0,0,0,
	0,1,0,
	1,0,0,
	0,1,0,
	0,0,1,
	0,1,0,
	0,0,0
};

static const uint32_t alienbullet2_down_3x7[] =
{																																								// image of buffered alien bullet
	0,0,0,
	0,1,0,
	0,0,1,
	0,1,0,
	1,0,0,
	0,1,0,
	0,0,0
};


static const uint32_t alienbullet2_gone_3x7[] =
{																																								// image of buffered alien bullet
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0
};

static const uint32_t v_14x10[] =
{																																								// image of buffered test image
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,1,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,1,0,0,0,1,0,0,
	0,0,0,0,0,0,0,0,1,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,1,0,0,0,0
};
static const uint32_t ll_14x10[] =
{																																								// image of buffered test image
	1,0,0,0,0,0,1,1,0,0,0,0,0,1,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	1,0,0,0,0,0,1,1,0,0,0,0,0,1

};


// prints an image to the screen
// image[] : the image to be printed
// width : the pixel width of the image
// height : the pixel height of the image
// starting_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
// color : desired color of the image
void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color);

// adds the image into a buffer for printing to the screen (performance increase)
// image[] : the image to be printed
// width : the pixel width of the image
// height : the pixel height of the image
// starting_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
// color : desired color of the image
void sprites_render_buffer(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, uint32_t color[3]);
